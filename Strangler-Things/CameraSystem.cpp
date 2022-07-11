#include "CGE.hpp"

#include "CameraSystem.hpp"


CameraSystem::CameraSystem()
{
	m_CameraObj = GameObject::Instantiate( "Camera"_N );
	Camera* CameraComponent = m_CameraObj.AddComponent< Camera >();

	ConsoleWindow* Window = ConsoleWindow::GetCurrentContext();
	CameraComponent->SetAspect( (float)Window->GetWidth() / Window->GetHeight() );

	CameraComponent->SetFOV( Math::Radians( 75.0f ) );
	CameraComponent->SetNearZ( 0.1f );
	CameraComponent->SetFarZ( 100.0f );
	Camera::SetMainCamera( CameraComponent );
	m_CameraObj.GetTransform()->SetLocalPosition( Vector3( 0.0f, 2.0f, 16.0f ) );

	m_CameraRotation = Vector2::Zero;
}

void CameraSystem::Update()
{
	Transform* CameraTransform = m_CameraObj.GetTransform();
	float D = Time::GetRealDeltaTime();
	float Speed = 1;

	if ( Input::IsKeyDown( KeyCode::Left  ) ) m_CameraRotation.y += Speed * D;
	if ( Input::IsKeyDown( KeyCode::Right ) ) m_CameraRotation.y -= Speed * D;
	if ( Input::IsKeyDown( KeyCode::Up    ) ) m_CameraRotation.x += Speed * D;
	if ( Input::IsKeyDown( KeyCode::Down  ) ) m_CameraRotation.x -= Speed * D;

	m_CameraRotation.x = Math::Clamp( m_CameraRotation.x, -Math::Radians( 89.9f ), Math::Radians( 89.9f ) );

	CameraTransform->SetLocalRotation( Quaternion::ToQuaternion( Vector3( m_CameraRotation, 0.0f ) ) );

	if ( Input::IsKeyDown( KeyCode::A ) ) CameraTransform->TranslateLocal( CameraTransform->GetLocalLeft()     * Speed * D );
	if ( Input::IsKeyDown( KeyCode::D ) ) CameraTransform->TranslateLocal( CameraTransform->GetLocalRight()    * Speed * D );
	if ( Input::IsKeyDown( KeyCode::W ) ) CameraTransform->TranslateLocal( CameraTransform->GetLocalForward()  * Speed * D );
	if ( Input::IsKeyDown( KeyCode::S ) ) CameraTransform->TranslateLocal( CameraTransform->GetLocalBackward() * Speed * D );
	if ( Input::IsKeyDown( KeyCode::Q ) ) CameraTransform->TranslateLocal( CameraTransform->GetLocalDown()     * Speed * D );
	if ( Input::IsKeyDown( KeyCode::E ) ) CameraTransform->TranslateLocal( CameraTransform->GetLocalUp()       * Speed * D );
}