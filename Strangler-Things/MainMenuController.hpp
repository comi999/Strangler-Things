#pragma once

#include "Component.hpp"
#include "Resource.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "MeshRenderer.hpp"

DefineComponent( MainMenuController, Component )
{
public:

	void Init( GameObject MenuObj )
	{
		if ( !BackgroundMesh.IsLoaded() )
		{
			BackgroundMesh = Resource::Load< Mesh >( "plane_mesh0"_H );
			BackgroundMesh->SetName( "background_mesh"_N );
		}

		if ( !BackgroundTexture.IsLoaded() )
		{
			BackgroundTexture = Resource::Load< Texture2D >( "menu_background"_H );
		}

		if ( !BackgroundMaterial.IsLoaded() )
		{
			Material BackgroundMat;
			BackgroundMat.SetName( "BackgroundMaterial"_N );
			BackgroundMat.AddTexture( "texture_diffuse"_N, BackgroundTexture );
			BackgroundMat.SetShader( Shader::Diffuse );

			BackgroundMaterial = BackgroundMat;
		}

		if ( !MenuItem1Texture.IsLoaded() )
		{
			MenuItem1Texture = Resource::Load< Texture2D >( "play_button_active"_H );
		}

		if ( !MenuItem1Material.IsLoaded() )
		{
			Material MenuItem1Mat;
			MenuItem1Mat.SetName( "MenuItem1Texture"_N );
			MenuItem1Mat.AddTexture( "texture_diffuse"_N, MenuItem1Texture );
			MenuItem1Mat.SetShader( Shader::Diffuse );

			MenuItem1Material = MenuItem1Mat;
		}

		MeshRenderer* Renderer = MenuObj.AddComponent< MeshRenderer >();
		Renderer->SetMaterial( BackgroundMaterial );
		Renderer->SetMesh( BackgroundMesh );
		auto* MenuTransform = MenuObj.GetTransform();
		MenuTransform->SetGlobalPosition( Vector3::Backward * 3.0f );
		MenuTransform->SetGlobalScale( Vector3( -11.0f, 1.0f, -8.0f ) );

		auto* CamTransform = Camera::GetMainCamera()->GetOwner()->GetTransform();
		CamTransform->SetLocalPosition( Vector3::Up );
		CamTransform->SetGlobalRotation( Vector3::Right * Math::Radians( -90.0f ) );

		GameObject MenuItem1 = GameObject::Instantiate( MenuObj );
		MeshRenderer* Renderer1 = MenuItem1.AddComponent< MeshRenderer >();
		Renderer1->SetMaterial( BackgroundMaterial );
		Renderer1->SetMesh( BackgroundMesh );
		auto* MenuTransform1 = MenuItem1.GetTransform();
		MenuTransform1->SetGlobalPosition( Vector3::Backward * 3.0f );// +Vector3::Up * 0.5f );
		MenuTransform1->SetGlobalScale( Vector3( 3.0f, 1.0f, -1.0f ) );
	}

	void Tick( float a_Progress )
	{
		
	}

private:

	inline static ResourceHandle< Mesh     > BackgroundMesh;
	inline static ResourceHandle< Material > BackgroundMaterial;
	inline static ResourceHandle< Texture2D > BackgroundTexture;

	inline static ResourceHandle< Material > MenuItem1Material;
	inline static ResourceHandle< Texture2D > MenuItem1Texture;
};