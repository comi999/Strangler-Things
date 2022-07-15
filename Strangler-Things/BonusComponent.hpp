#pragma once

#include "Component.hpp"
#include "Delegate.hpp"


DefineComponent( BonusComponent, Component )
{
public:
	inline static Delegate<> OnAnyCollected;
	Delegate<> OnCollected;

};