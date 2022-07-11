#pragma once

#include "IGraphicsPopulator.hpp"


class SceneGraphicsPopulator : public IGraphicsPopulator
{
public:

	void Populate( GameObject& ) override;
};