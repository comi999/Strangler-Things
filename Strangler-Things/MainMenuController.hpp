#pragma once

#include "Component.hpp"
#include "Resource.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "MeshRenderer.hpp"

#define MakeMatPair( Name ) \
if ( !##Name##_texture.IsLoaded() ) \
		{\
##Name##_texture = Resource::Load< Texture2D >( #Name##_H );\
		}\
		if ( !##Name##_material.IsLoaded() )\
		{\
			Material BackgroundMat;\
			BackgroundMat.SetName( "BackgroundMaterial"_N );\
			BackgroundMat.AddTexture( "texture_diffuse"_N, ##Name##_texture );\
			BackgroundMat.SetShader( Shader::Diffuse );\
			##Name##_material = BackgroundMat;\
		}\

#define MakeMatPairDec( Name )\
inline static ResourceHandle< Material > ##Name##_material;\
inline static ResourceHandle< Texture2D > ##Name##_texture;\

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

		MakeMatPair( menu_background );
		MakeMatPair( play_button_active );
		MakeMatPair( play_button );
		MakeMatPair( instruction_button_active );
		MakeMatPair( instruction_button );

		// Background and camera
		MeshRenderer* Renderer = MenuObj.AddComponent< MeshRenderer >();
		Renderer->SetMaterial( menu_background_material );
		Renderer->SetMesh( BackgroundMesh );
		auto* MenuTransform = MenuObj.GetTransform();
		MenuTransform->SetGlobalPosition( Vector3::Backward * 3.0f );
		MenuTransform->SetGlobalScale( Vector3( -11.0f, 1.0f, -8.0f ) );

		GameObject MenuItem1 = GameObject::Instantiate( MenuObj );
		MeshRenderer* Renderer1 = MenuItem1.AddComponent< MeshRenderer >();
		Renderer1->SetMaterial( play_button_active_material );
		Renderer1->SetMesh( BackgroundMesh );
		auto* MenuTransform1 = MenuItem1.GetTransform();
		MenuTransform1->SetGlobalPosition( Vector3::Up * 0.5f + Vector3::Backward * -0.34f );
		MenuTransform1->SetGlobalScale( Vector3( 0.32f, 1.0f, -0.11f ) );

		// Instruction button
		/*GameObject MenuItem2 = GameObject::Instantiate( MenuObj );
		MeshRenderer* Renderer2 = MenuItem2.AddComponent< MeshRenderer >();
		Renderer2->SetMaterial( instruction_button_active_material );
		Renderer2->SetMesh( BackgroundMesh );
		auto* MenuTransform2 = MenuItem2.GetTransform();
		MenuTransform2->SetGlobalPosition( Vector3::Up * 0.5f + Vector3::Backward * -0.35f );
		MenuTransform2->SetGlobalScale( Vector3( 0.32f, 1.0f, -0.11f ) );*/
	}

	void Tick( float a_Progress )
	{
		
	}

private:

	inline static ResourceHandle< Mesh     > BackgroundMesh;

	MakeMatPairDec( menu_background );
	MakeMatPairDec( play_button_active );
	MakeMatPairDec( play_button );
	MakeMatPairDec( instruction_button_active );
	MakeMatPairDec( instruction_button );
};