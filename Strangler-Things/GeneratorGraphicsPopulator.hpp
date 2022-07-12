#pragma once

#include "IGraphicsPopulator.hpp"


class GeneratorGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};