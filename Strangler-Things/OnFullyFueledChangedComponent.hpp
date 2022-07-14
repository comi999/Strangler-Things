#pragma once

#include <functional>

#include "Component.hpp"
#include "GameplaySystem.hpp"
#include "Invoker.hpp"


DefineComponent( OnFullyFueledChangedComponent, Component )
{
public:
	void Init( std::function< void() > a_Callback )
	{
		m_Callback = a_Callback;
		GeneratorSystem::OnFullyFueledChanged += m_Callback;

	}

	void OnDestroy()
	{
		GeneratorSystem::OnFullyFueledChanged -= m_Callback;
	}

private:
	// TODO: Make Actions work here instead of std::functions
	std::function< void() > m_Callback;

};