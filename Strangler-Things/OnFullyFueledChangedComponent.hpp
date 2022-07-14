#pragma once

#include <functional>

#include "Component.hpp"
#include "GameplaySystem.hpp"
#include "Invoker.hpp"


DefineComponent( OnFullyFueledChangedComponent, Component )
{
public:
	IOnFullyFueledChangedComponent() : m_Initd( false ) { }

	IOnFullyFueledChangedComponent( IOnFullyFueledChangedComponent&& a_Movee )
		: m_Initd( false )
	{
		if ( a_Movee.m_Initd )
		{
			Init( a_Movee.m_Callback );
		}
	}

	void operator= ( IOnFullyFueledChangedComponent&& a_Movee )
	{
		if ( a_Movee.m_Initd )
		{
			Init( a_Movee.m_Callback );
		}
	}

	void Init( std::function< void() > a_Callback )
	{
		m_Callback = a_Callback;
		GeneratorSystem::OnFullyFueledChanged += m_Callback;
		m_Initd = true;

	}

	~IOnFullyFueledChangedComponent()
	{
		if ( m_Initd )
		{
			GeneratorSystem::OnFullyFueledChanged -= m_Callback;
		}
	}

private:
	bool m_Initd;
	// TODO: Make Actions work here instead of std::functions
	std::function< void() > m_Callback;

};