#pragma once

#include "GameObject.hpp"


class GeneratorSystem
{
public:
	void InitForNewMatch();
	void Update();

private:
	int m_FuelRequired;
	int m_FuelConsumed;

};