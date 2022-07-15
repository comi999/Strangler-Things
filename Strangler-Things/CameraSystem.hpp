#pragma once

#include "GameObject.hpp"
#include "Math.hpp"


class CameraSystem
{
public:
	static void Follow( GameObject );

	CameraSystem();

	void Update();

private:
	static CameraSystem* s_I;

	GameObject m_CameraObj;
	Vector2 m_CameraRotation;
	GameObject m_Target;
	Vector3 m_TargetPoint;
	bool m_IsOnMenu;

	void UpdateIsOnMenu( bool );

};