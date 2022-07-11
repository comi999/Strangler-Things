#pragma once

#include "IGraphicsPopulator.hpp"


class LowWallGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};