#pragma once

#include "IGraphicsPopulator.hpp"


class HorizontalExitGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};