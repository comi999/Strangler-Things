#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "File.hpp"
#include "GameObject.hpp"
#include "Invoker.hpp"
#include "Math.hpp"

#include "GraphicsPopulator.hpp"

#include <iostream>

using namespace std;


GameObject SceneObj;

void CreateHighWall( Vector3Int Coord )
{
	GameObject HighWallObj = GameObject::Instantiate( "HighWall"_N, SceneObj );
	GraphicsPopulator::I.HighWall( HighWallObj );
	HighWallObj.GetTransform()->SetLocalPosition( Coord );
}

void CreateLowWall( Vector3Int Coord )
{
	GameObject LowWallObj = GameObject::Instantiate( "LowWall"_N, SceneObj );
	GraphicsPopulator::I.LowWall( LowWallObj );
	LowWallObj.GetTransform()->SetLocalPosition( Coord );
}

map< char, Action< Vector3Int > > TileCallbacks =
{
	make_pair( ' ', Action< Vector3Int >() ),
	make_pair( '.', Action< Vector3Int >() ),
	make_pair( '#', CreateHighWall ),
	make_pair( '-', CreateHighWall )
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

	string Tilemap = FileToString( TilemapPath );
	stringstream TilemapLines( Tilemap );

	int Y = 0;
	while ( !TilemapLines.eof() )
	{
		int X = 0;

		string Line;
		getline( TilemapLines, Line );

		for ( char C : Line )
		{
			if ( C == '\r' )
				continue;

			auto Iter = TileCallbacks.find( C );
			_STL_ASSERT(
				Iter != TileCallbacks.end(),
				( "Unsupported tile found in tilemap: " + to_string( C ) ).c_str()
			);

			Iter->second( Vector3Int( X, 0, 20 - Y ) );

			++X;
		}

		++Y;
	}
}