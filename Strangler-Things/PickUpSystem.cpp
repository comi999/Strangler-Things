#include "CGE.hpp"

#include "AudioSource.hpp"
#include "PickUpAbleComponent.hpp"
#include "PickUpSystem.hpp"
#include "PlayerComponent.hpp"


void PickUpSystem::Update()
{
	static float MaxPickUpDistanceSq = Math::Pow( 1.0f, 2.0f );
	static float PickUpOverheadHeight = 1.33f;

	bool KeyPressedNow = Input::IsKeyDown( KeyCode::Space );
	bool KeyJustPressed = KeyPressedNow && !m_WasSpaceDown;
	m_WasSpaceDown = KeyPressedNow;
	if ( ! KeyJustPressed )
	{
		return;
	}

	for ( auto Player : Component::GetComponents< PlayerComponent >() )
	{
		GameObject PlayerObj = *Player->GetOwner();
		Transform* PlayerTfm = Player->GetTransform();
		Vector3 PlayerPos = PlayerTfm->GetGlobalPosition();

		if ( Player->m_PickedUpObject.IsValid() )
		{
			Transform* PUTransform = Player->m_PickedUpObject.GetTransform();

			PUTransform->SetParent( PlayerTfm->GetParent() );
			PUTransform->SetLocalPosition( PlayerTfm->GetLocalPosition() );

			Player->m_PickedUpObject.GetComponent< PickUpAbleComponent >()->Holder = GameObject( -1 );
			Player->m_PickedUpObject = GameObject( -1 );
			Player->OnDroppedObject.InvokeAll();
		}
		else
		{
			for ( auto PickUpAble : Component::GetComponents< PickUpAbleComponent >() )
			{
				if ( PickUpAble->Holder.IsValid() )
				{
					continue;
				}

				GameObject PickUpObj = *PickUpAble->GetOwner();
				Transform* PUTransform = PickUpObj.GetTransform();

				if ( Math::DistanceSqrd( PlayerPos, PUTransform->GetGlobalPosition() ) <= MaxPickUpDistanceSq )
				{
					PUTransform->SetParent( PlayerTfm );
					PUTransform->SetLocalPosition( Vector3( 0.0f, PickUpOverheadHeight, 0.0f ) );

					PickUpAble->Holder = PlayerObj;
					Player->m_PickedUpObject = PickUpObj;
					Player->OnPickedUpObject.InvokeAll();
					break;
				}
			}
		}
	}
}