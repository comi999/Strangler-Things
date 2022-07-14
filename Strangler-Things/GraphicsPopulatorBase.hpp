#pragma once

#include "GameObject.hpp"


class TentacleNode;

class GraphicsPopulatorBase
{
public:
	virtual void Scene( GameObject a_Object, Hash a_LevelName );
	virtual void Abyss( GameObject a_Object );
	virtual void Floor( GameObject a_Object );
	virtual void HighWall( GameObject a_Object );
	virtual void LowWall( GameObject a_Object );
	virtual void HorizontalExit( GameObject a_Object );
	virtual void Player( GameObject a_Object );
	virtual void TentacleStart( GameObject a_Object );
	virtual void TentacleNode_( GameObject a_Object, TentacleNode* a_TentacleNode );
	virtual void Random( GameObject a_Object );
	virtual void Fuel( GameObject a_Object );
	virtual void Generator( GameObject a_Object );
	virtual void Bonus( GameObject a_Object );
	virtual void RandomBlocker( GameObject a_Object );

};