#pragma once

#include "Time.hpp"
#include "Resource.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Component.hpp"
#include "MeshRenderer.hpp"

DefineComponent( SplashScreenController, Component )
{
public:

	void Init( GameObject a_MenuObject )
	{
		if ( !SplashMesh.IsLoaded() )
		{
			SplashMesh = Resource::Load< Mesh >( "plane_mesh0"_H );
			SplashMesh->SetName( "something"_N );
		}

		if ( !SplashShader.IsLoaded() )
		{
			SplashShader = Shader::Spotlight;// Shader( "splash_shader"_N, "Vertex_Texel", "Fragment_Splash" );
		}

		if ( !SplashTexture.IsLoaded() )
		{
			SplashTexture = Resource::Load< Texture2D >( "splash"_H );
		}

		if ( !SplashMaterial.IsLoaded() )
		{
			Material Mat;
			Mat.SetName( "SplashMat"_N );
			Mat.AddProperty( "intensity"_N, 0.0f );
			Mat.AddTexture( "texture_diffuse"_N, SplashTexture );
			Mat.SetShader( *SplashShader );

			SplashMaterial = Mat;
		}

		MeshRenderer* Renderer = a_MenuObject.AddComponent< MeshRenderer >();
		Renderer->SetMaterial( SplashMaterial );
		Renderer->SetMesh( SplashMesh );

		auto* MenuTransform = a_MenuObject.GetTransform();
		MenuTransform->SetGlobalPosition( Vector3::Backward * 3.5f );
		//MenuTransform->SetGlobalScale( Vector3::One * 10.0f );
		MenuTransform->SetLocalScale( Vector3( -11.0f, 1.0f, -6.5f ) );
	}

	void TickSplash( float a_Progress )
	{
		/*float Intensity = Math::Abs( 0.8f * Math::Sin( a_Progress * 0.2f * Math::Radians( 180.0f ) ) );
		SplashMaterial->SetProperty( "intensity"_H, Intensity );*/

		Light* Sun = Light::GetSun();

		if ( !Sun )
		{
			GameObject SunObject = GameObject::Instantiate();
			Sun = SunObject.AddComponent< Light >();
			Light::SetSun( Sun );
		}

		Sun->SetAmbient( Vector3::Up * 3.0f );
		Sun->SetDirection( Math::Normalize( Vector3::Down + Vector3::Left * Math::Sin( a_Progress ) ) );
	}

private:

	inline static ResourceHandle< Mesh > SplashMesh;
	inline static ResourceHandle< Material > SplashMaterial;
	inline static ResourceHandle< Texture2D > SplashTexture;
	inline static ResourceHandle< Shader > SplashShader;
};