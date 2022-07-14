#include "File.hpp"

#include "GameplaySystem.hpp"
#include "Level.hpp"


void GameplaySystem::StartLevel( Hash a_LevelName )
{
	s_I->m_PendingLevel = a_LevelName;
	s_I->m_PendingTicks = 200;

	auto* CurLevel = Level::GetActiveLevel();
	if ( CurLevel != nullptr )
	{
		CurLevel->Deactivate();
	}
	else
	{
		s_I->StartPendingLevelNow();
	}
}

GameplaySystem::GameplaySystem()
	: m_PickUpSystem(), m_PendingTicks( 0 )
{
	s_I = this;

	m_PendingLevel = ""_H;

}

void GameplaySystem::Update()
{
	if ( m_PendingLevel != ""_H )
	{
		--m_PendingTicks;
		if ( m_PendingTicks <= 0 )
		{
			StartPendingLevelNow();
		}
		return;
	}

	if ( m_RestartLevelSystem.Update() )
	{
		StartLevel( Level::GetActiveLevel()->GetName() );
		return;
	}

	m_PlayerMovementSystem.Update();
	m_GeneratorSystem.Update();
	m_PickUpSystem.Update();
	m_TentacleGrowthSystem.Update();
	m_LateGameplayUpdateSystem.Update();

}

void GameplaySystem::StartPendingLevelNow()
{
	Level::SetActiveLevel( m_PendingLevel );
	m_GeneratorSystem.InitForNewLevel();

	m_PendingLevel = ""_H;

}