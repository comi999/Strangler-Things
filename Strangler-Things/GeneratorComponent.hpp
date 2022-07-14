#pragma once

#include "Component.hpp"
#include "Delegate.hpp"


DefineComponent( GeneratorComponent, Component )
{
public:
	Delegate<> OnConsumedFuel;

};