#include "File.hpp"

#include "GameplaySystem.hpp"
#include "PlayerMovementSystem.hpp"
#include "SceneLoader.hpp"


GameplaySystem* GameplaySystem::s_I;

void GameplaySystem::StartGameplay( Path& a_TilemapPath )
{
	s_I->m_Match = std::make_shared< Match >();
	LoadScene( a_TilemapPath );

}

GameplaySystem::GameplaySystem()
{
	s_I = this;
	m_Match = nullptr;

}

void GameplaySystem::Update()
{
	m_PlayerMovementSystem.Update();
	m_PickUpSystem.Update();

}