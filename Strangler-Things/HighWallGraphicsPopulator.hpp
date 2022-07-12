#pragma once

#include "IGraphicsPopulator.hpp"


class HighWallGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};