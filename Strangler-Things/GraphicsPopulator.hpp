#pragma once

#include "GraphicsPopulatorBase.hpp"


class TentacleNode;

class GraphicsPopulator : public GraphicsPopulatorBase
{
public:
	void Scene( GameObject a_Object, Hash a_LevelName ) override;
	void Abyss( GameObject a_Object ) override;
	void Floor( GameObject a_Object ) override;
	void HighWall( GameObject a_Object ) override;
	void LowWall( GameObject a_Object ) override;
	void HorizontalExit( GameObject a_Object ) override;
	void Player( GameObject a_Object ) override;
	void TentacleStart( GameObject a_Object ) override;
	void TentacleNode_( GameObject a_Object, TentacleNode* a_TentacleNode ) override;
	void Fuel( GameObject a_Object ) override;
	void Generator( GameObject a_Object ) override;
	void Bonus( GameObject a_Object ) override;
	void RandomBlocker( GameObject a_Object ) override;

};

static GraphicsPopulator GP;