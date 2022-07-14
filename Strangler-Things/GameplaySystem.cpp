#include "File.hpp"

#include "GameplaySystem.hpp"
#include "Level.hpp"


void GameplaySystem::StartLevel( Hash a_LevelName )
{
	Level::SetActiveLevel( a_LevelName );

	s_I->m_GeneratorSystem.InitForNewLevel();

}

GameplaySystem::GameplaySystem()
	: m_PickUpSystem()
{
	s_I = this;

}

void GameplaySystem::Update()
{
	m_PlayerMovementSystem.Update();
	m_GeneratorSystem.Update();
	m_PickUpSystem.Update();
	m_LateGameplayUpdateSystem.Update();

}