#pragma once

#include "GameObject.hpp"
#include "Prefab.hpp"

#include "OnExitStatusChangedComponent.hpp"


class GraphicsPopulatorBase
{
public:
	virtual void Scene( GameObject a_Object ) { };
	virtual void Abyss( GameObject a_Object ) { };
	virtual void Floor( GameObject a_Object ) { };
	virtual void HighWall( GameObject a_Object )
	{
		static ResourceHandle< Material > HighWallMaterial = []()
		{
			Material Temp = Material::LitFlatColour;
			Temp.SetName( "HighWallMaterial"_N );
			Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::WHITE );
			Temp.SetShader( Shader::LitFlatColour );
			return Temp;
		}( );

		Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	
		for ( int i = 0; i < 2; ++i )
		{
			GameObject NewHighWall = Prefab::Instantiate( *Cube );
			MeshRenderer* NewRenderer = NewHighWall.GetComponentInChild< MeshRenderer >();
			NewRenderer->SetMaterial( HighWallMaterial );
			Transform* NewTransform = NewHighWall.GetTransform();
			NewTransform->SetGlobalPositionY( i );
			NewTransform->SetParent( a_Object, false );
		}
	};
	virtual void LowWall( GameObject a_Object )
	{
		static ResourceHandle< Material > LowWallMaterial = []()
		{
			Material Temp = Material::LitFlatColour;
			Temp.SetName( "LowWallMaterial"_N );
			Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::WHITE );
			Temp.SetShader( Shader::LitFlatColour );
			return Temp;
		}();

		Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
		GameObject NewLowWall = Prefab::Instantiate( *Cube );
		MeshRenderer* NewRenderer = NewLowWall.GetComponentInChild< MeshRenderer >();
		NewRenderer->SetMaterial( LowWallMaterial );
		Transform* NewTransform = NewLowWall.GetTransform();
		NewTransform->SetParent( a_Object, false );
	};
	virtual void HorizontalExit( GameObject a_Object )
	{
		auto CreateExitCube = []( GameObject a_Parent )
		{
			static ResourceHandle< Material > ExitMaterial = []()
			{
				Material Temp = Material::LitFlatColour;
				Temp.SetName( "ExitMaterial"_N );
				Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::BLUE );
				Temp.SetShader( Shader::LitFlatColour );
				return Temp;
			}();

			Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
			GameObject NewCube = Prefab::Instantiate( *Cube );
			MeshRenderer* NewRenderer = NewCube.GetComponentInChild< MeshRenderer >();
			NewRenderer->SetMaterial( ExitMaterial );
			Transform* NewTransform = NewCube.GetTransform();
			NewTransform->SetParent( a_Parent, false );

			return NewCube;
		};

		Vector3 DoorCoord = a_Object.GetTransform()->GetLocalPosition();

		GameObject LeftCubeObj = CreateExitCube( a_Object );
		Transform* LeftCube = LeftCubeObj.GetTransform();
		LeftCube->SetLocalPositionX( -1.0f );
		LeftCube->SetLocalScale( Vector3( 1.0f, 2.2f, 1.2f ) );
	
		GameObject RightCubeObj = CreateExitCube( a_Object );
		Transform* RightCube = RightCubeObj.GetTransform();
		RightCube->SetLocalPositionX( 1.0f );
		RightCube->SetLocalScale( Vector3( 1.0f, 2.2f, 1.2f ) );
	
		GameObject TopCubeObj = CreateExitCube( a_Object );
		Transform* TopCube = TopCubeObj.GetTransform();
		TopCube->SetLocalPositionY( 1.5f );
		TopCube->SetLocalScale( Vector3( 1.0f, 0.6f, 1.2f ) );

		GameObject ClosedCubeObj = CreateExitCube( a_Object );
		{
			Transform* ClosedCube = ClosedCubeObj.GetTransform();
			ClosedCube->SetLocalScale( Vector3( 1.0f, 1.5f, 1.0f ) );
		}

		GameObject::Instantiate( a_Object )
			.AddComponent< OnExitStatusChangedComponent >( )
			->Init([=]()
			{
				Transform* ClosedCube = ( (GameObject)ClosedCubeObj ).GetTransform();

				if (GameplaySystem::IsExitOpen())
				{
					ClosedCube->SetLocalScale( Vector3::Zero );
				}
				else
				{
					ClosedCube->SetLocalScale( Vector3( 1.0f, 1.5f, 1.0f ) );
				}
			});
	};
	virtual void Player( GameObject a_Object )
	{
		static ResourceHandle< Material > PlayerMaterial = []()
		{
			Material Temp = Material::LitFlatColour;
			Temp.SetName( "PlayerMaterial"_N );
			Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::GREEN );
			Temp.SetShader( Shader::LitFlatColour );
			return Temp;
		}();

		Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
		GameObject NewPlayer = Prefab::Instantiate( *Cube );
		MeshRenderer* NewRenderer = NewPlayer.GetComponentInChild< MeshRenderer >();
		NewRenderer->SetMaterial( PlayerMaterial );

		Transform* NewTransform = NewPlayer.GetTransform();
		NewTransform->SetParent( a_Object, false );
	};
	virtual void TentacleStart( GameObject a_Object ) { };
	virtual void Random( GameObject a_Object ) { };
	virtual void Fuel( GameObject a_Object )
	{
		static ResourceHandle< Material > FuelMaterial = []()
		{
			Material Temp = Material::LitFlatColour;
			Temp.SetName( "FuelMaterial"_N );
			Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::LIGHT_ORANGE );
			Temp.SetShader( Shader::LitFlatColour );
			return Temp;
		}();

		Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
		GameObject NewFuel = Prefab::Instantiate( *Cube );
		MeshRenderer* NewRenderer = NewFuel.GetComponentInChild< MeshRenderer >();
		NewRenderer->SetMaterial( FuelMaterial );

		Transform* NewTransform = NewFuel.GetTransform();
		NewTransform->SetParent( a_Object, false );
		NewTransform->SetLocalScale( Vector3::One * 0.66f );
	};
	virtual void Generator( GameObject a_Object )
	{
		static ResourceHandle< Material > GeneratorMaterial = []()
		{
			Material Temp = Material::LitFlatColour;
			Temp.SetName( "GeneratorMaterial"_N );
			Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::LIGHT_BLUE );
			Temp.SetShader( Shader::LitFlatColour );
			return Temp;
		}();

		Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
		GameObject NewGenerator = Prefab::Instantiate( *Cube );
		MeshRenderer* NewRenderer = NewGenerator.GetComponentInChild< MeshRenderer >();
		NewRenderer->SetMaterial( GeneratorMaterial );

		Transform* NewTransform = NewGenerator.GetTransform();
		NewTransform->SetParent( a_Object, false );
		NewTransform->SetLocalScale( Vector3( 0.33f, 3.0f, 0.33f ) );
	};
	virtual void Bonus( GameObject a_Object ) { };
	virtual void RandomBlocker( GameObject a_Object ) { };

};