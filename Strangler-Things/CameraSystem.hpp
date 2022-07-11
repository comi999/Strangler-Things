#pragma once

#include "GameObject.hpp"
#include "Math.hpp"


class CameraSystem
{
public:
	CameraSystem();

	void Update();

private:
	GameObject m_CameraObj;
	Vector2 m_CameraRotation;
};