#pragma once

#include <functional>
#include <memory>

#include "GeneratorSystem.hpp"
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
	// TODO: Multicast delegate, noting that the existing Action / Delegate doesn't handle captures
	inline static std::function< void() > OnExitStatusChanged;

	static void StartMatch( Path& );
	static std::weak_ptr< Match > GetMatch()
	{
		return std::weak_ptr< Match >( s_I->m_Match );
	}
	static bool IsExitOpen()
	{
		return s_I->m_GeneratorSystem.IsFullyFueled();
	}

	GameplaySystem();

	void Update();

private:
	inline static GameplaySystem* s_I;

	std::shared_ptr< Match > m_Match;
	GeneratorSystem m_GeneratorSystem;
	PickUpSystem m_PickUpSystem;
	PlayerMovementSystem m_PlayerMovementSystem;

};