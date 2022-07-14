#pragma once

#include "GameObject.hpp"
#include "Delegate.hpp"


class GeneratorSystem
{
public:
	Delegate<> OnFuelConsumptionChanged;
	Delegate<> OnFullyFueled;

	void InitForNewMatch();
	void Update();
	bool IsFullyFueled();

private:
	int m_FuelRequired;
	int m_FuelConsumed;

};