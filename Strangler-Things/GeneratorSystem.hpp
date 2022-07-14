#pragma once

#include "GameObject.hpp"
#include "Delegate.hpp"


class GeneratorSystem
{
public:
	inline static Delegate<> OnFullyFueledChanged;
	static bool IsFullyFueled();

	Delegate<> OnFuelConsumptionChanged;
	Delegate<> OnFullyFueledHere;
	GeneratorSystem();
	void InitForNewLevel();
	void Update();

private:
	inline static GeneratorSystem* s_I;

	int m_FuelRequired;
	int m_FuelConsumed;

};