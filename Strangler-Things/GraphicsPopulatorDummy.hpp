#pragma once


class GameObject;

class GraphicsPopulatorRaw
{
public:
	virtual void LowWall( GameObject& Obj );
	virtual void HighWall( GameObject& Obj );
};