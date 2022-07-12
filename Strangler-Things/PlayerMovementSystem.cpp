#include "CGE.hpp"

#include "PlayerComponent.hpp"
#include "PlayerMovementSystem.hpp"


void PlayerMovementSystem::Update()
{
	static float Speed = 2;
	float D = Time::GetRealDeltaTime();

	for ( auto Player : Component::GetComponents< PlayerComponent >() )
	{
		auto Transform = Player->GetOwner()->GetTransform();
		auto Offset = Vector3::Zero;
		
		if (
			Input::IsKeyDown( KeyCode::Left  ) ||
			Input::IsKeyDown( KeyCode::A     )
		) Offset.x -= Speed * D;
		if (
			Input::IsKeyDown( KeyCode::Right ) ||
			Input::IsKeyDown( KeyCode::D     )
		) Offset.x += Speed * D;
		if (
			Input::IsKeyDown( KeyCode::Up    ) ||
			Input::IsKeyDown( KeyCode::W     )
		) Offset.z += Speed * D;
		if (
			Input::IsKeyDown( KeyCode::Down  ) ||
			Input::IsKeyDown( KeyCode::S     )
		) Offset.z -= Speed * D;

		// TODO: Moves the player to world 0 initially if using TranslateGlobal instead of Local
		Transform->TranslateLocal( Offset );
	}
}