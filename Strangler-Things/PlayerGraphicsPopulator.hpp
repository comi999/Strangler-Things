#pragma once

#include "IGraphicsPopulator.hpp"


class PlayerGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};