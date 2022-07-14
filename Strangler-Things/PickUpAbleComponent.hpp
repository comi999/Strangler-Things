#pragma once

#include "Component.hpp"


DefineComponent( PickUpAbleComponent, Component )
{
public:
	Delegate<> OnPickedUp;
	Delegate<> OnDropped;
	GameObject Holder;

};