#pragma once

#include "Atlas.hpp"
#include "GeneratorSystem.hpp"
#include "LateGameplayUpdateSystem.hpp"
#include "PickUpSystem.hpp"
#include "PlayerMovementSystem.hpp"


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

};