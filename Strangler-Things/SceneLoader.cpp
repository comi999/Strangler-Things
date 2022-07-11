#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "File.hpp"
#include "GameObject.hpp"
#include "Invoker.hpp"
#include "Math.hpp"

#include "HighWallGraphicsPopulator.hpp"
#include "LowWallGraphicsPopulator.hpp"

#include <iostream>

using namespace std;

GameObject SceneObj;

std::map< Hash, Action< GameObject& > > GraphicsPopulators = 
{
	std::make_pair( "HighWall"_H, Action< GameObject& >( new HighWallGraphicsPopulator(), &HighWallGraphicsPopulator::Populate ) ),
	std::make_pair( "LowWall"_H, Action< GameObject& >( new LowWallGraphicsPopulator(), &LowWallGraphicsPopulator::Populate ) )
};

template < Hash _ObjectName >
void CreateMapObject( Vector3Int a_Coord )
{
	GameObject NewMapObj = GameObject::Instantiate( SceneObj );
	GraphicsPopulators[ _ObjectName ]( NewMapObj );
	NewMapObj.GetTransform()->SetLocalPosition( a_Coord );
}

map< char, Action< Vector3Int > > TileCallbacks =
{
	make_pair( ' ', Action< Vector3Int >() ),
	make_pair( '.', Action< Vector3Int >() ),
	make_pair( '#', CreateMapObject< "HighWall"_H  > ),
	make_pair( '-', CreateMapObject< "LowWall"_H > )
};

string FileToString( const Path& FilePath )
{
	File F = FilePath;
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

void LoadScene( const Path& TilemapPath )
{
	_STL_ASSERT( !SceneObj.IsValid(), "Scene unloading not yet supported" );
	_STL_ASSERT( TilemapPath.IsFile(), "TilemapPath isn't pointing to a file" );

	SceneObj = GameObject::Instantiate( "Scene"_N );

	// Create Sun
	GameObject SunObject = GameObject::Instantiate( "Sun"_N, SceneObj );
	Light* SunComponent = SunObject.AddComponent< Light >();
	SunComponent->SetDirection( Math::Normalize( Vector3::Down + Vector3::Right + Vector3::Forward * 0.3f ) );
	Light::SetSun( SunComponent );

	string Tilemap = FileToString( TilemapPath );
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
			/*_STL_ASSERT(
				Iter != TileCallbacks.end(),
				( "Unsupported tile found in tilemap: " + to_string( C ) ).c_str()
			);*/

			if ( Iter != TileCallbacks.end() )
			{
				Iter->second( Vector3Int( X, 0, 20 - Y ) );
			}
		}

		++Y;
	}
}