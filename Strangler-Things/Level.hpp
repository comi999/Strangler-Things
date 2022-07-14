#pragma once

#include <sstream>
#include <iostream>

#include "Atlas.hpp"
#include "File.hpp"
#include "GameObject.hpp"
#include "Math.hpp"


enum class LevelObject : uint8_t
{
	ABYSS           = '.',
	FLOOR           = ' ',
	HIGH_WALL       = '#',
	LOW_WALL        = '-',
	HORIZONTAL_EXIT = '_',
	PLAYER          = 'p',
	TENTACLE_START  = 't',
	RANDOM_BLOCKER  = '?',
	FUEL            = 'f',
	GENERATOR       = 'g',
	BONUS           = 's',
};

class Level
{
public:

	static Level* GetLevel( Hash a_Name );
	static Level* GetActiveLevel();
	static bool SetActiveLevel( Hash a_Name );
	static void Preload( std::vector< std::pair< Hash, std::string > > a_Levels );
	
	Level( const std::string& a_Tilemap, Vector2UInt a_Dimensions );
	Vector2UInt GetLevelSize() const;
	LevelObject GetTile( Vector2UInt a_Coordinate ) const;
	void ResetAtlas();

	Atlas& GetAtlas();
	const Atlas& GetAtlas() const;

private:

	static void CreateNewLevel( Hash a_Name, std::string a_LevelPath );

	Vector2UInt m_LevelSize;
	std::string m_LevelData;
	Atlas       m_LevelAtlas;

	inline static std::map< Hash, Level > s_Levels;
	inline static Level*                  s_ActiveLevel;

};