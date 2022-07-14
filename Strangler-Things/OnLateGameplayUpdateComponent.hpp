#pragma once

#include "Component.hpp"
#include "GameplaySystem.hpp"
#include "Invoker.hpp"


class LateGameplayUpdateSystem;

DefineComponent( OnLateGameplayUpdateComponent, Component )
{
	friend LateGameplayUpdateSystem;

public:
	void Init( Action<> a_Callback )
	{
		m_Callback = a_Callback;
	}

private:
	Action<> m_Callback;

};