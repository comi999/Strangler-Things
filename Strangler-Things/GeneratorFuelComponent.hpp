#pragma once

#include "Component.hpp"
#include "Delegate.hpp"


DefineComponent( GeneratorFuelComponent, Component )
{
public:
	inline static Delegate<> OnAnyEatenByTentacle;

	Delegate<> OnEatenByTentacle;

};