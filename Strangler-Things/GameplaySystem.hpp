#pragma once

#include "PlayerMovementSystem.hpp"


class GameplaySystem
{
public:
	void Update();

private:
	PlayerMovementSystem m_PlayerMovementSystem;

};