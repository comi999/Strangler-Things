#include "GameObject.hpp"
#include "Prefab.hpp"

#include "GraphicsPopulatorBase.hpp"
#include "OnFullyFueledChangedComponent.hpp"
#include "OnLateGameplayUpdateComponent.hpp"
#include "TentacleComponent.hpp"


void GraphicsPopulatorBase::Scene( GameObject a_Object, Hash a_LevelName ) { };
void GraphicsPopulatorBase::Abyss( GameObject a_Object ) { };
void GraphicsPopulatorBase::Floor( GameObject a_Object ) { };
void GraphicsPopulatorBase::HighWall( GameObject a_Object )
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
void GraphicsPopulatorBase::LowWall( GameObject a_Object )
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
void GraphicsPopulatorBase::HorizontalExit( GameObject a_Object )
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
		.AddComponent< OnFullyFueledChangedComponent >( )
		->Init([=]()
		{
			Transform* ClosedCube = ( (GameObject)ClosedCubeObj ).GetTransform();

			if ( GeneratorSystem::IsFullyFueled() )
			{
				ClosedCube->SetLocalScale( Vector3::Zero );
			}
			else
			{
				ClosedCube->SetLocalScale( Vector3( 1.0f, 1.5f, 1.0f ) );
			}
		});
};
void GraphicsPopulatorBase::Player( GameObject a_Object )
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
void GraphicsPopulatorBase::TentacleStart( GameObject a_Object ) { };
void GraphicsPopulatorBase::TentacleNode_( GameObject a_Object, TentacleNode* a_TentacleNode )
{
	static ResourceHandle< Material > TentacleMaterial = []()
	{
		Material Temp = Material::UnlitFlatColour;
		Temp.SetName( "TentacleMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour( 3, 65, 55, 255 ) );
		Temp.SetShader( Shader::UnlitFlatColour );
		return Temp;
	}();

	Prefab* Cube = Resource::Load< Prefab >( "cylinder"_H ).Assure();
	GameObject NewTentacle = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewTentacle.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( TentacleMaterial );

	Transform* NewTransform = NewTentacle.GetTransform();
	NewTransform->SetParent( a_Object, false );
	NewTransform->SetLocalScale( Vector3( 0.3f, 1.0f, 0.3f ) );
	// NewTransform->RotateLocal( Quaternion( Vector3( Math::Radians( 90.0f ), 0.0f, 0.0f ) ) );

	RotationOrder Order = RotationOrder::XYZ;
	Vector3 Rot = Vector3( Math::Radians( -90.0f ), Math::ATan< float >( -a_TentacleNode->PreviousDirection.x, a_TentacleNode->PreviousDirection.y ), 0.0f );
	Quaternion Q = Quaternion::ToQuaternion( Rot, Order );
	NewTransform->SetGlobalRotation( Q );
};
void GraphicsPopulatorBase::Fuel( GameObject a_Object )
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
void GraphicsPopulatorBase::Generator( GameObject a_Object )
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
void GraphicsPopulatorBase::Bonus( GameObject a_Object )
{
	static ResourceHandle< Material > BonusMaterial = []()
	{
		Material Temp = Material::UnlitFlatColour;
		Temp.SetName( "BonusMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::LIGHT_YELLOW );
		Temp.SetShader( Shader::UnlitFlatColour );
		return Temp;
	}();

	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	GameObject NewBonus = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewBonus.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( BonusMaterial );

	Transform* NewTransform = NewBonus.GetTransform();
	NewTransform->SetParent( a_Object, false );
	NewTransform->SetLocalScale( Vector3( 0.5f, 0.5f, 0.5f ) );
	NewTransform->SetLocalPositionY( 0.75f );

	NewBonus.AddComponent< OnLateGameplayUpdateComponent >( )
		->Init([=]()
		{
			static Quaternion Rotator = Quaternion::ToQuaternion( Vector3( 0.1f, 0.1f, 0.1f ) );
			// TODO: Why does this lock up on global, or grow and crash on local?
			( (GameObject)NewBonus ).GetTransform()->RotateGlobal( Rotator );
		});
};
void GraphicsPopulatorBase::RandomBlocker( GameObject a_Object ) { };