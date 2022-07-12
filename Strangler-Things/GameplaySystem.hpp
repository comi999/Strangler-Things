#pragma once

#include <memory>

#include "GridOccupancySystem.hpp"
#include "PickUpSystem.hpp"
#include "PlayerMovementSystem.hpp"


class Path;

class Match
{
public:
	GridOccupancySystem& GetGridOccupancySystem() { return m_GridOccupancySystem; }

private:
	GridOccupancySystem m_GridOccupancySystem;

};

class GameplaySystem
{
public:
	static void StartGameplay( Path& );
	static std::weak_ptr< Match > GetMatch()
	{
		return std::weak_ptr< Match >( s_I->m_Match );
	}

	GameplaySystem();

	void Update();

private:
	static GameplaySystem* s_I;

	std::shared_ptr< Match > m_Match;
	PickUpSystem m_PickUpSystem;
	PlayerMovementSystem m_PlayerMovementSystem;

};