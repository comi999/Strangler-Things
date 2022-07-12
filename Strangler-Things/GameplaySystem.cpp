#include "File.hpp"

#include "GameplaySystem.hpp"
#include "SceneLoader.hpp"


GameplaySystem* GameplaySystem::s_I;

void GameplaySystem::StartMatch( Path& a_TilemapPath )
{
	s_I->m_Match = std::make_shared< Match >();
	LoadScene( a_TilemapPath );

	s_I->m_GeneratorSystem.InitForNewMatch();

}

GameplaySystem::GameplaySystem()
{
	s_I = this;
	m_Match = nullptr;

}

void GameplaySystem::Update()
{
	m_PlayerMovementSystem.Update();
	m_GeneratorSystem.Update();
	m_PickUpSystem.Update();

}