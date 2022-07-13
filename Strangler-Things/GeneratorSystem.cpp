#include <vector>

#include "CGE.hpp"

#include "AudioSource.hpp"
#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "GeneratorSystem.hpp"


void GeneratorSystem::InitForNewMatch()
{
	auto Fuels = Component::GetComponents< GeneratorFuelComponent >();
	m_FuelRequired = Fuels.size();
	m_FuelConsumed = 0;

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
		Vector3 FuelPos = FuelTfm->GetLocalPosition();
		FuelPos.y = 0;

		for ( auto& Generator : Generators )
		{
			auto GeneratorPos = Generator->GetTransform()->GetLocalPosition();
			GeneratorPos.y = 0.0f;
			if ( Math::DistanceSqrd( GeneratorPos, FuelPos ) <= MaxFuelDistanceSq )
			{
                auto* GeneratorAudio = Generator->GetOwner()->GetComponent<AudioSource>();
				GeneratorAudio->Play();

				GameObject::Destroy( *Fuel->GetOwner() );
				++m_FuelConsumed;
				break;
			}
		}
	}

	OnFuelConsumptionChanged.InvokeAll();
}

bool GeneratorSystem::IsFullyFueled()
{
	return m_FuelConsumed >= m_FuelRequired;
}