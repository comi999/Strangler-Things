#pragma once
#include "Resource.hpp"
#include "Prefab.hpp"

class GameObject;

class IGraphicsPopulator
{
public:

	virtual void Populate( GameObject& a_Populate ) { }
};