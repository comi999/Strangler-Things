#include "CGE.hpp"

#include "AudioListener.hpp"
#include "CameraSystem.hpp"
#include "MenuSystem.hpp"


CameraSystem* CameraSystem::s_I = nullptr;

CameraSystem::CameraSystem()
{
	s_I = this;

	m_CameraObj = GameObject::Instantiate( "Camera"_N );
	Camera* CameraComponent = m_CameraObj.AddComponent< Camera >();
	m_CameraObj.AddComponent< AudioListener >();

	ConsoleWindow* Window = ConsoleWindow::GetCurrentContext();
	CameraComponent->SetAspect( (float)Window->GetWidth() / Window->GetHeight() );

	CameraComponent->SetNearZ( 0.1f );
	CameraComponent->SetFarZ( 100.0f );
	Camera::SetMainCamera( CameraComponent );

	m_CameraRotation = Vector2::Zero;
	m_TargetPoint = Vector3::Zero;

	UpdateIsOnMenu( false );
}

void CameraSystem::Update()
{
	static Vector3 Offset = Vector3( 0.0f, 8.0f, -3.0f );

	if (m_Target.IsValid())
	{
		m_TargetPoint = m_Target.GetTransform()->GetGlobalPosition();
	}

	m_CameraObj.GetTransform()->SetLocalPosition( m_TargetPoint + Offset );

	if ( MenuSystem::I().Active != m_IsOnMenu )
	{
		UpdateIsOnMenu( !m_IsOnMenu );
	}

}

void CameraSystem::Follow( GameObject a_Obj )
{
	_STL_ASSERT( s_I != nullptr, "" );
	s_I->m_Target = a_Obj;
	s_I->m_TargetPoint = a_Obj.GetTransform()->GetGlobalPosition();

}

void CameraSystem::UpdateIsOnMenu( bool a_IsOnMenu )
{
	m_IsOnMenu = a_IsOnMenu;

 	auto CameraComponent = m_CameraObj.GetComponent< Camera >();
	auto CamTransform = m_CameraObj.GetTransform();

	if ( m_IsOnMenu )
	{
		CameraComponent->SetFOV( Math::Radians( 82.0f ) );
		CamTransform->SetLocalPosition( Vector3::Up * 5.0f );
		CamTransform->SetLocalRotation( Vector3::Right * Math::Radians( -90.0f ) );
	}
	else
	{
		CameraComponent->SetFOV( Math::Radians( 75.0f ) );
		CamTransform->SetLocalPosition( Vector3::Up );
		CamTransform->SetGlobalRotation( Quaternion::ToQuaternion( Vector3( Math::Radians( -50.0f ), 0.0f, 0.0f ) ) );
	}

}