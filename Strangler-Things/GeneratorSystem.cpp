#include <vector>

#include "CGE.hpp"

#include "AudioSource.hpp"
#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "GeneratorSystem.hpp"


GeneratorSystem::GeneratorSystem()
{
	s_I = this;

}

void GeneratorSystem::InitForNewLevel()
{
	auto Fuels = Component::GetComponents< GeneratorFuelComponent >();
	m_FuelRequired = Fuels.size();
	m_FuelConsumed = 0;
	
	OnFullyFueledChanged.InvokeAll();
	OnFuelConsumptionChanged.InvokeAll();

}

void GeneratorSystem::Update()
{
	static float MaxFuelDistanceSq = Math::Pow( 0.66f, 2.0f );

	if ( m_FuelConsumed == m_FuelRequired )
	{
		return;
	}

	auto Generators = Component::GetComponents< GeneratorComponent >();
	auto Fuels = Component::GetComponents< GeneratorFuelComponent >();

	for ( auto Fuel : Fuels )
	{
		Transform* FuelTfm = Fuel->GetTransform();
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		// TODO: Use global position so it'll find things the player is picking up
		Vector3 FuelPos = FuelTfm->GetLocalPosition();
		FuelPos.y = 0.0f;

		for ( auto& Generator : Generators )
		{
			auto GeneratorPos = Generator->GetTransform()->GetLocalPosition();
			GeneratorPos.y = 0.0f;
			if ( Math::DistanceSqrd( GeneratorPos, FuelPos ) <= MaxFuelDistanceSq )
			{
				GameObject::Destroy( *Fuel->GetOwner() );
				++m_FuelConsumed;

				if (m_FuelConsumed == m_FuelRequired)
				{
					OnFullyFueledChanged.InvokeAll();
					OnFullyFueledHere.InvokeAll();
				}

				Generator->OnConsumedFuel.InvokeAll();
				break;
			}
		}
	}

	OnFuelConsumptionChanged.InvokeAll();
}

bool GeneratorSystem::IsFullyFueled()
{
	return s_I->m_FuelConsumed >= s_I->m_FuelRequired;
}