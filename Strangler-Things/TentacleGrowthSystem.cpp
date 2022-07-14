#include "CGE.hpp"

#include "TentacleComponent.hpp"
#include "TentacleGrowthSystem.hpp"


void TentacleGrowthSystem::Update()
{
	float D = Time::GetRealDeltaTime();

	for ( auto* Tentacle : Component::GetExactComponents< TentacleComponent >() )
	{
		Tentacle->m_NextGrowthAfterSeconds -= D;
		if (Tentacle->m_NextGrowthAfterSeconds <= 0)
		{
			Tentacle->Grow();
		}
	}

}