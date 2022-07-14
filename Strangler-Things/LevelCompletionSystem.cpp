#include "Component.hpp"

#include "ExitComponent.hpp"
#include "GameplaySystem.hpp"
#include "GeneratorSystem.hpp"
#include "Level.hpp"
#include "LevelCompletionSystem.hpp"
#include "MathHelpers.hpp"
#include "PlayerComponent.hpp"


void LevelCompletionSystem::InitForNewLevel()
{
	auto Players = Component::GetComponents< PlayerComponent >();
	m_PlayersRequired = Players.size();
	m_PlayersComplete = 0;

}

void LevelCompletionSystem::Update()
{
	if (
		m_PlayersComplete >= m_PlayersRequired ||
		!GeneratorSystem::IsFullyFueled()
	)
	{
		return;
	}

	for ( auto& Player : Component::GetComponents< PlayerComponent >() )
	{
		Vector3 PlayerPos = Player->GetTransform()->GetGlobalPosition();
		Vector3UInt PlayerCoord = MathHelpers::PositionToCoord3( PlayerPos );

		for ( auto& Exit : Component::GetComponents< ExitComponent >() )
		{
			Vector3 ExitPos = Exit->GetTransform()->GetGlobalPosition();
			Vector3UInt ExitCoord = MathHelpers::PositionToCoord3( ExitPos );

			if ( PlayerCoord == ExitCoord )
			{
				Player->OnReachedExit.InvokeAll();
				GameObject::Destroy( *Player->GetOwner() );

				++m_PlayersComplete;
			}
		}
	}

	if ( m_PlayersComplete >= m_PlayersRequired )
	{
		std::vector< Hash > OrderedLevelNames = GameplaySystem::GetOrderedLevels();
		auto Iter = std::find( OrderedLevelNames.begin(), OrderedLevelNames.end(), Level::GetActiveLevel()->GetName() );

		bool IsFinalLevel = *Iter == OrderedLevelNames.back();

		OnLevelComplete.InvokeAll( IsFinalLevel );

		if ( !IsFinalLevel )
		{
			GameplaySystem::StartLevel( *( Iter + 1 ) );
		}
	}
}
