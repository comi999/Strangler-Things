#include "CGE.hpp"

#include "GameplaySystem.hpp"
#include "PlayerComponent.hpp"
#include "PlayerMovementSystem.hpp"
#include "AudioSource.hpp"
#include "Level.hpp"


void PlayerMovementSystem::Update()
{
	static float Speed = 2.0f;
	static float CollisionSize = 0.5f;

	float D = Time::GetRealDeltaTime();
	auto& Atlas = Level::GetActiveLevel()->GetAtlas();

	for ( auto Player : Component::GetComponents< PlayerComponent >() )
	{
		auto Transform = Player->GetOwner()->GetTransform();
		auto Offset = Vector3::Zero;
		Vector3 CurrentPosition = Transform->GetLocalPosition();

		if (
			(
				Input::IsKeyDown( KeyCode::Left  ) ||
				Input::IsKeyDown( KeyCode::A     )
			) &&
			Atlas.CheckNode( CurrentPosition + Vector3::Left * CollisionSize )
		)
		{
			Offset.x -= Speed * D;
		}
		if (
			(
				Input::IsKeyDown( KeyCode::Right ) ||
				Input::IsKeyDown( KeyCode::D     )
			) &&
			Atlas.CheckNode( CurrentPosition + Vector3::Right * CollisionSize )
		)
		{
			Offset.x += Speed * D;
		}
		if (
			(
				Input::IsKeyDown( KeyCode::Up    ) ||
				Input::IsKeyDown( KeyCode::W     )
			) &&
			Atlas.CheckNode( CurrentPosition + Vector3::Forward * CollisionSize )
		)
		{
			Offset.z += Speed * D;
		}
		if (
			(
				Input::IsKeyDown( KeyCode::Down  ) ||
				Input::IsKeyDown( KeyCode::S     )
			) &&
			Atlas.CheckNode( CurrentPosition + Vector3::Backward * CollisionSize )
		) Offset.z -= Speed * D;

		if (Offset != Vector3::Zero)
		{
			Transform->TranslateLocal(Offset);
		}
		
		Player->m_LastMovementOffset = Offset;
	}
}