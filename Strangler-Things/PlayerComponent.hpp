#pragma once

#include "Component.hpp"
#include "Delegate.hpp"


class PickUpSystem;
class PlayerMovementSystem;

DefineComponent( PlayerComponent, Component )
{
	friend PickUpSystem;
	friend PlayerMovementSystem;

public:
	Delegate<> OnPickedUpObject;
	Delegate<> OnDroppedObject;

	Vector3 GetLastMovementOffset() { return m_LastMovementOffset; }

private:
	GameObject m_PickedUpObject;
	Vector3 m_LastMovementOffset;

};