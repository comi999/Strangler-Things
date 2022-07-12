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
#include "GameplaySystem.hpp"
#include "HighWallGraphicsPopulator.hpp"
#include "LowWallGraphicsPopulator.hpp"
#include "PlayerComponent.hpp"
#include "PlayerGraphicsPopulator.hpp"
#include "SceneGraphicsPopulator.hpp"

#include <iostream>


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

template < Hash _ObjectName >
GameObject CreateMapObjectWithGridOccupancy( Vector3Int a_Coord )
{
	GameplaySystem::GetMatch().lock()->GetGridOccupancySystem().RegisterOccupied( a_Coord );
	return CreateMapObject< _ObjectName >( a_Coord );
}

GameObject CreatePlayer( Vector3Int a_Coord )
{
	GameObject Player = CreateMapObject< "Player"_H >( a_Coord );
	Player.AddComponent< PlayerComponent >();

	CameraSystem::Follow( Player );

	return Player;
}

std::map< char, Invoker< GameObject, Vector3Int > > TileCallbacks =
{
	std::make_pair( ' ', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( '.', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( '#', CreateMapObjectWithGridOccupancy< "HighWall"_H  > ),
	std::make_pair( '-', CreateMapObjectWithGridOccupancy< "LowWall"_H > ),
	std::make_pair( 'p', CreatePlayer ),
	std::make_pair( '_', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( 't', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( '?', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( 'f', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( 'g', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( 's', Invoker< GameObject, Vector3Int >() )
};

std::string FileToString( const Path& a_FilePath )
{
	File F = a_FilePath;
	if ( F.Open() )
	{
		std::string S;
		S.resize( F.Size() );
		F.Read( S.data(), S.size() );

		// TODO: Close on destructor?
		F.Close();

		return S;
	}

	throw std::exception( "Could not open file" );
}

void LoadScene( const Path& a_TilemapPath )
{
	_STL_ASSERT( !SceneObj.IsValid(), "Scene unloading not yet supported" );
	_STL_ASSERT( a_TilemapPath.IsFile(), "TilemapPath isn't pointing to a file" );

	SceneObj = GameObject::Instantiate( "Scene"_N );

	SceneGraphicsPopulator().Populate( SceneObj );

	std::string Tilemap = FileToString( a_TilemapPath );
	std::stringstream TilemapLines( Tilemap );

	int Y = 0;
	while ( !TilemapLines.eof() )
	{
		int X = 0;

		std::string Line;
		std::getline( TilemapLines, Line );

		for ( auto Begin = Line.begin(), End = Line.end(); Begin != End; ++Begin, ++X )
		{
			if ( *Begin == '\r' )
			{
				continue;
			}

			auto Iter = TileCallbacks.find( *Begin );
			_STL_ASSERT(
				Iter != TileCallbacks.end(),
				( "Unsupported tile found in tilemap: " + std::to_string( *Begin ) + " (" + std::to_string( X ) + ", " + std::to_string ( Y ) + ") " ).c_str()
			);

			if ( Iter != TileCallbacks.end() )
			{
				Iter->second( Vector3Int( X, 0, 20 - Y ) );
			}
		}

		++Y;
	}
}