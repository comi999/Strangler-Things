#include <vector>

#include "CGE.hpp"

#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "GeneratorSystem.hpp"


void GeneratorSystem::InitForNewMatch()
{
	auto Fuels = Component::GetComponents< GeneratorFuelComponent >();
	m_FuelRequired = Fuels.size();
	m_FuelConsumed = 0;

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

	std::vector< Vector3 > GeneratorPositions( Generators.size() );
	size_t I = 0;
	for ( auto Begin = Generators.begin(), End = Generators.end(); Begin != End; ++Begin, ++I )
	{
		GeneratorPositions[ I ] = (*Begin)->GetTransform()->GetLocalPosition();
		GeneratorPositions[ I ].y = 0;
	}

	for ( auto Fuel : Fuels )
	{
		Transform* FuelTfm = Fuel->GetTransform();
		// TODO: Use global position so it'll find things the player is picking up
		Vector3 FuelPos = FuelTfm->GetLocalPosition();
		FuelPos.y = 0;

		for ( auto& GeneratorPos : GeneratorPositions )
		{
			if ( Math::DistanceSqrd( GeneratorPos, FuelPos ) <= MaxFuelDistanceSq )
			{
				// TODO: Delete the fuel game object properly
				GameObject* FuelObj = Fuel->GetOwner();
				Fuel->DestroyComponent< GeneratorFuelComponent >( *FuelObj );
				FuelTfm->SetGlobalPosition( Vector3( -100.0f, -100.0f, -100.0f ) );
				FuelTfm->SetLocalScale( Vector3::Zero );

				++m_FuelConsumed;
				break;
			}
		}
	}

	if ( m_FuelConsumed == m_FuelRequired )
	{
		_STL_ASSERT( false, "YOU WIN!" );
	}
}