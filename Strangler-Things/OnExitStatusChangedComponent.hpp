#pragma once

#include <functional>

#include "Component.hpp"
#include "GameplaySystem.hpp"
#include "Invoker.hpp"


DefineComponent( OnExitStatusChangedComponent, Component )
{
public:
	void Init( std::function< void() > a_Callback )
	{
		m_Callback = a_Callback;
		GameplaySystem::OnExitStatusChanged += m_Callback;
		m_Initd = true;
	}

	~IOnExitStatusChangedComponent()
	{
		if ( m_Initd )
		{
			GameplaySystem::OnExitStatusChanged -= m_Callback;
		}
	}

private:
	bool m_Initd;
	// TODO: Make Actions work here instead of std::functions
	std::function< void() > m_Callback;

};