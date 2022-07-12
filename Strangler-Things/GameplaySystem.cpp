#include "File.hpp"

#include "GameplaySystem.hpp"
#include "SceneLoader.hpp"


void HandleFuelConsumptionChanged()
{
	GameplaySystem::OnExitStatusChanged();
}

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

	m_GeneratorSystem.OnFuelConsumptionChanged += HandleFuelConsumptionChanged;

}

void GameplaySystem::Update()
{
	m_PlayerMovementSystem.Update();
	m_GeneratorSystem.Update();
	m_PickUpSystem.Update();

}