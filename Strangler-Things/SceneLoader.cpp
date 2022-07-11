#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "File.hpp"
#include "GameObject.hpp"
#include "Invoker.hpp"
#include "Math.hpp"

#include "CameraSystem.hpp"
#include "HighWallGraphicsPopulator.hpp"
#include "LowWallGraphicsPopulator.hpp"
#include "PlayerGraphicsPopulator.hpp"
#include "SceneGraphicsPopulator.hpp"

#include <iostream>

using namespace std;


GameObject SceneObj;

std::map< Hash, Action< GameObject& > > GraphicsPopulators = 
{
	std::make_pair( "HighWall"_H, Action< GameObject& >( new HighWallGraphicsPopulator(), &HighWallGraphicsPopulator::Populate ) ),
	std::make_pair( "LowWall"_H, Action< GameObject& >( new LowWallGraphicsPopulator(), &LowWallGraphicsPopulator::Populate ) ),
	std::make_pair( "Player"_H, Action< GameObject& >( new PlayerGraphicsPopulator(), &PlayerGraphicsPopulator::Populate ) )
};

template < Hash _ObjectName >
GameObject CreateMapObject( Vector3Int a_Coord )
{
	GameObject NewMapObj = GameObject::Instantiate( SceneObj );
	// TODO: Changing from Local to Global position here causes everything spawned to be located at origin?!?
	// (Not important for this game, but just a strange bug for the Lengine)
	NewMapObj.GetTransform()->SetLocalPosition( a_Coord );
	GraphicsPopulators[ _ObjectName ]( NewMapObj );

	return NewMapObj;
}

GameObject CreatePlayer( Vector3Int a_Coord )
{
	GameObject Player = CreateMapObject< "Player"_H >( a_Coord );

	CameraSystem::I()->Follow( Player );

	return Player;
}

map< char, Invoker< GameObject, Vector3Int > > TileCallbacks =
{
	make_pair( ' ', Invoker< GameObject, Vector3Int >() ),
	make_pair( '.', Invoker< GameObject, Vector3Int >() ),
	make_pair( '#', CreateMapObject< "HighWall"_H  > ),
	make_pair( '-', CreateMapObject< "LowWall"_H > ),
	make_pair( 'p', CreatePlayer ),
	make_pair( '_', Invoker< GameObject, Vector3Int >() ),
	make_pair( 't', Invoker< GameObject, Vector3Int >() ),
	make_pair( '?', Invoker< GameObject, Vector3Int >() ),
	make_pair( 'f', Invoker< GameObject, Vector3Int >() ),
	make_pair( 'g', Invoker< GameObject, Vector3Int >() ),
	make_pair( 's', Invoker< GameObject, Vector3Int >() )
};

string FileToString( const Path& a_FilePath )
{
	File F = a_FilePath;
	if ( F.Open() )
	{
		string S;
		S.resize( F.Size() );
		F.Read( S.data(), S.size() );

		// TODO: Close on destructor?
		F.Close();

		return S;
	}

	throw exception( "Could not open file" );
}

void LoadScene( const Path& a_TilemapPath )
{
	_STL_ASSERT( !SceneObj.IsValid(), "Scene unloading not yet supported" );
	_STL_ASSERT( a_TilemapPath.IsFile(), "TilemapPath isn't pointing to a file" );

	SceneObj = GameObject::Instantiate( "Scene"_N );

	SceneGraphicsPopulator().Populate( SceneObj );

	string Tilemap = FileToString( a_TilemapPath );
	stringstream TilemapLines( Tilemap );

	int Y = 0;
	while ( !TilemapLines.eof() )
	{
		int X = 0;

		string Line;
		getline( TilemapLines, Line );

		for ( auto Begin = Line.begin(), End = Line.end(); Begin != End; ++Begin, ++X )
		{
			if ( *Begin == '\r' )
			{
				continue;
			}

			auto Iter = TileCallbacks.find( *Begin );
			_STL_ASSERT(
				Iter != TileCallbacks.end(),
				( "Unsupported tile found in tilemap: " + to_string( *Begin ) + " (" + to_string( X ) + ", " + to_string ( Y ) + ") " ).c_str()
			);

			if ( Iter != TileCallbacks.end() )
			{
				Iter->second( Vector3Int( X, 0, 20 - Y ) );
			}
		}

		++Y;
	}
}