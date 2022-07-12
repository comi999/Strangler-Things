#pragma once

#include "IGraphicsPopulator.hpp"


class FuelGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};