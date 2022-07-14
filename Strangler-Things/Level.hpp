#pragma once
#include <sstream>
#include <iostream>

#include "File.hpp"
#include "GameObject.hpp"
#include "Math.hpp"

#include "Atlas.hpp"

enum class LevelObject : uint8_t
{
	ABYSS          = '.',
	FLOOR          = ' ',
	HIGH_WALL      = '#',
	LOW_WALL       = '-',
	EXIT           = '_',
	PLAYER_START   = 'p',
	TENTACLE_START = 't',
	RANDOM         = '?',
	FUEL           = 'f',
	GENERATOR      = 'g',
	BONUS          = 's'
};



class Level
{
public:

	Level( const std::string& a_Tilemap, Vector2UInt a_Dimensions );
	Vector2UInt GetLevelSize() const;
	LevelObject GetLevelObject( Vector2UInt a_Coordinate ) const;
	void PopulateScene( GameObject a_Scene );
	void ResetAtlas();
	Atlas& GetAtlas();
	const Atlas& GetAtlas() const;
	static Level* CreateNewLevel( Hash a_Name, const std::string& a_Tilemap, Vector2UInt a_Size );
	static Level* GetLevel( Hash a_Name );
	static Level* GetActiveLevel();
	static bool SetActiveLevel( Hash a_Name );

private:

	Vector2UInt m_LevelSize;
	std::string m_LevelData;
	Atlas       m_LevelAtlas;

	inline static std::map< Hash, Level > s_Levels;
	inline static Level*           s_ActiveLevel;
};