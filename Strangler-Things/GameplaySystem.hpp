#pragma once

#include <vector>

#include "BonusSystem.hpp"
#include "GeneratorSystem.hpp"
#include "LateGameplayUpdateSystem.hpp"
#include "LevelCompletionSystem.hpp"
#include "PickUpSystem.hpp"
#include "PlayerMovementSystem.hpp"
#include "RestartLevelSystem.hpp"
#include "TentacleSystem.hpp"


class Path;

class GameplaySystem
{
public:
	static void StartLevel( Hash );
	inline static const std::vector< Hash > GetOrderedLevels()
	{
		return s_OrderedLevels;
	}

	GameplaySystem();

	void Update();

private:
	inline static GameplaySystem* s_I;
	static std::vector< Hash > s_OrderedLevels;

	BonusSystem m_BonusSystem;
	GeneratorSystem m_GeneratorSystem;
	LateGameplayUpdateSystem m_LateGameplayUpdateSystem;
	LevelCompletionSystem m_LevelCompletionSystem;
	PickUpSystem m_PickUpSystem;
	PlayerMovementSystem m_PlayerMovementSystem;
	RestartLevelSystem m_RestartLevelSystem;
	TentacleSystem m_TentacleGrowthSystem;

	Hash m_PendingLevel;
	int m_PendingTicks;

	void StartPendingLevelNow();

};