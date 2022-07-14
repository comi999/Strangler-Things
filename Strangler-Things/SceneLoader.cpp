#include <format>
#include <fstream>
#include <map>
#include <stdexcept>

#include "File.hpp"
#include "GameObject.hpp"
#include "Invoker.hpp"
#include "Math.hpp"
#include "AudioSource.hpp"

#include "Level.hpp"

#include <iostream>


GameObject SceneObj;

std::string FileToString( const Path& a_FilePath )
{
	File F = a_FilePath;

	if ( F.Open() )
	{
		std::string S;
		S.resize( F.Size() );
		F.Read( S.data(), S.size() );
		F.Close();

		return S;
	}

	throw std::exception( "Could not open file" );
}

void LoadScene( const Path& a_TilemapPath )
{
	if ( SceneObj.IsValid() )
	{
		GameObject::Destroy( SceneObj );
	}

	SceneObj = GameObject::Instantiate( "Scene"_N );

	_STL_ASSERT( a_TilemapPath.IsFile(), "TilemapPath isn't pointing to a file" );

	std::string TileMap = FileToString( a_TilemapPath );

	std::stringstream TilemapStream( TileMap );
	std::vector< std::string > Rows;
	size_t Width = 0;
	
	// Create collection of rows and store largest Width.
	while ( !TilemapStream.eof() )
	{
		auto& NewRow = Rows.emplace_back();
		std::getline( TilemapStream, NewRow );
		Width = Math::Max( Width, NewRow.size() );
	}
	
	// Pad out all rows to Width.
	for ( auto& Row : Rows )
	{
		Row.resize( Width );
	}
	
	// Get the size of the level.
	Vector2UInt LevelSize = { Width, Rows.size() };
	
	// Compile all rows into level data.
	TileMap.clear();
	
	for ( auto Begin = Rows.rbegin(), End = Rows.rend(); Begin != End; ++Begin )
	{
		TileMap += *Begin;
	}
	Level* Level1 = Level::CreateNewLevel( "Level1"_H, TileMap, LevelSize );
	Level::SetActiveLevel( "Level1"_H );
	Level1->PopulateScene( SceneObj );
}