#pragma once

#include "Atlas.hpp"
#include "GeneratorSystem.hpp"
#include "LateGameplayUpdateSystem.hpp"
#include "PickUpSystem.hpp"
#include "PlayerMovementSystem.hpp"
#include "RestartLevelSystem.hpp"
#include "TentacleSystem.hpp"


class Path;

class GameplaySystem
{
public:
	static void StartLevel( Hash );

	GameplaySystem();

	void Update();

private:
	inline static GameplaySystem* s_I;

	GeneratorSystem m_GeneratorSystem;
	LateGameplayUpdateSystem m_LateGameplayUpdateSystem;
	PickUpSystem m_PickUpSystem;
	PlayerMovementSystem m_PlayerMovementSystem;
	RestartLevelSystem m_RestartLevelSystem;
	TentacleSystem m_TentacleGrowthSystem;

	Hash m_PendingLevel;
	int m_PendingTicks;

	void StartPendingLevelNow();

};