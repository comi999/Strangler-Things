#include "CGE.hpp"
#include "Component.hpp"
#include "Math.hpp"

#include "BonusComponent.hpp"
#include "BonusSystem.hpp"
#include "PlayerComponent.hpp"


void BonusSystem::Update()
{
	static float MaxPickUpDistanceSq = Math::Pow( 0.66f, 2.0f );

	for ( auto& Bonus : Component::GetComponents< BonusComponent >() )
	{
		Vector3 BonusPos = Bonus->GetTransform()->GetLocalPosition();

		for ( auto& Player : Component::GetComponents< PlayerComponent >() )
		{
			Vector3 PlayerPos = Player->GetTransform()->GetLocalPosition();

			if ( Math::DistanceSqrd( BonusPos, PlayerPos ) <= MaxPickUpDistanceSq )
			{
				BonusComponent::OnAnyCollected.InvokeAll();
				Bonus->OnCollected.InvokeAll();
				GameObject::Destroy( *Bonus->GetOwner() );
			}
		}
	}

}