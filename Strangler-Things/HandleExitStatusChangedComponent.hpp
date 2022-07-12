#pragma once

#include <functional>

#include "Component.hpp"
#include "GameplaySystem.hpp"
#include "Invoker.hpp"


DefineComponent( HandleExitStatusChangedComponent, Component )
{
public:
	void Init( std::function< void() > a_Callback )
	{
		m_Callback = a_Callback;
		// GameplaySystem::OnExitStatusChanged += MakeInvoker< void >( [&]() { m_Callback(); } );
		m_Initd = true;
	}

	~IHandleExitStatusChangedComponent()
	{
		if ( m_Initd )
		{
			// GameplaySystem::OnExitStatusChanged -= m_Callback;
		}
	}

private:
	bool m_Initd;
	Action<> m_Callback;

};