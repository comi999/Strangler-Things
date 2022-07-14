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
		}

		if ( !SplashShader.IsLoaded() )
		{
			SplashShader = Shader( "splash_shader"_N, "Vertex_Default", "Fragment_Splash" );
		}

		if ( !SplashTexture.IsLoaded() )
		{
			SplashTexture = Resource::Load< Texture2D >( "splash"_H );
		}

		if ( !SplashMaterial.IsLoaded() )
		{
			SplashMaterial = Material();
			SplashMaterial->SetName( "SplashMaterial"_N );
			SplashMaterial->AddProperty( "intensity"_N, 0.0f );
			SplashMaterial->AddTexture( "texture_diffuse"_N, SplashTexture );
			SplashMaterial->SetShader( *SplashShader );
		}

		MenuObject = a_MenuObject;
		MeshRenderer* Renderer = MenuObject.AddComponent< MeshRenderer >();
		Renderer->SetMaterial( SplashMaterial );
		Renderer->SetMesh( SplashMesh );
	}

	void TickSplash( float a_Progress )
	{
		float Intensity = 0.2f + Math::Abs( 0.8f * Math::Sin( a_Progress * 0.2f * Math::Radians( 180.0f ) ) );
		SplashMaterial->SetProperty( "intensity"_H, Intensity );
	}

private:

	GameObject MenuObject;
	inline static ResourceHandle< Mesh > SplashMesh;
	inline static ResourceHandle< Material > SplashMaterial;
	inline static ResourceHandle< Texture2D > SplashTexture;
	inline static ResourceHandle< Shader > SplashShader;
};