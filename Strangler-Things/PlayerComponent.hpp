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
	Delegate<> OnReachedExit;
	Delegate<> OnPickedUpObject;
	Delegate<> OnDroppedObject;
	float TimeSpentWalking = 0.0f;
	bool AnimationState = 0.0f;
	Vector2Int PreviousDirection = Vector2Int::Down;

	Vector3 GetLastMovementOffset() const { return m_LastMovementOffset; }

private:
	GameObject m_PickedUpObject;
	Vector3 m_LastMovementOffset;

};