#include "CGE.hpp"

#include "LateGameplayUpdateSystem.hpp"
#include "OnLateGameplayUpdateComponent.hpp"


void LateGameplayUpdateSystem::Update()
{
	for ( OnLateGameplayUpdateComponent* C : Component::GetExactComponents< OnLateGameplayUpdateComponent >() )
		C->m_Callback();

}