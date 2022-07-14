#pragma once

#include <functional>

#include "Component.hpp"
#include "GameplaySystem.hpp"
#include "Invoker.hpp"


class LateGameplayUpdateSystem;

DefineComponent( OnLateGameplayUpdateComponent, Component )
{
	friend LateGameplayUpdateSystem;

public:
	void Init( std::function< void() > a_Callback )
	{
		m_Callback = a_Callback;
	}

private:
	std::function< void() > m_Callback;

};