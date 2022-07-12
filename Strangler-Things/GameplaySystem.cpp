#include "GameplaySystem.hpp"
#include "PlayerMovementSystem.hpp"


void GameplaySystem::Update()
{
	m_PlayerMovementSystem.Update();

}