#pragma once

#include "GameObject.hpp"
#include "Math.hpp"


class CameraSystem
{
public:
	static CameraSystem* I() { return s_I; }

	CameraSystem();

	void Update();
	void Follow( GameObject );

private:
	static CameraSystem* s_I;

	GameObject m_CameraObj;
	Vector2 m_CameraRotation;

};