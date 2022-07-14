#include "CGE.hpp"

#include "GeneratorFuelComponent.hpp"
#include "TentacleComponent.hpp"
#include "TentacleSystem.hpp"


void TentacleSystem::Update()
{
	float D = Time::GetRealDeltaTime();

	auto Tentacles = Component::GetExactComponents< TentacleComponent >();

	for ( auto* Tentacle : Tentacles )
	{
		Tentacle->m_NextGrowthAfterSeconds -= D;
		if (Tentacle->m_NextGrowthAfterSeconds <= 0)
		{
			Tentacle->Grow();
		}
	}
	
	auto Fuels = Component::GetComponents< GeneratorFuelComponent >();

	for ( auto Fuel : Fuels )
	{
		Transform* FuelTfm = Fuel->GetTransform();
		// TODO: Use global position!
		Vector3 FuelPos = FuelTfm->GetLocalPosition();
		FuelPos.y = 0.0f;
		Vector3UInt FuelCoord = MathHelpers::PositionToCoord3( FuelPos );

		for ( auto& Tentacle : Tentacles )
		{
			for ( auto& LeafNodeCoord : Tentacle->m_LeafNodes )
			{
				if ( LeafNodeCoord == FuelCoord )
				{
					GeneratorFuelComponent::OnAnyEatenByTentacle.InvokeAll();
					Fuel->OnEatenByTentacle.InvokeAll();

					GameObject::Destroy( *Fuel->GetOwner() );
				}
			}
		}
	}

}