#pragma once


class AudioPopulatorBase
{
public:
	virtual void Global() { };
	virtual void Scene( GameObject a_Object ) { };
	virtual void HorizontalExit( GameObject a_Object ) { };
	virtual void Player( GameObject a_Object ) { };
	virtual void TentacleStart( GameObject a_Object ) { };
	virtual void Random( GameObject a_Object ) { };
	virtual void Fuel( GameObject a_Object ) { };
	virtual void Generator( GameObject a_Object ) { };
	virtual void Bonus( GameObject a_Object ) { };

};