#include "Light.hpp"
#include "Shader.hpp"
#include "Prefab.hpp"
#include "Resource.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Time.hpp"

#include "GraphicsPopulator.hpp"
#include "TentacleComponent.hpp"
#include "OnLateGameplayUpdateComponent.hpp"
#include "PlayerComponent.hpp"
#include "OnFullyFueledChangedComponent.hpp"


void GraphicsPopulator::Scene( GameObject a_Object, Hash a_LevelName )
{
	// Create Sun
	GameObject SunObject = GameObject::Instantiate( "Sun"_N, a_Object );
	Light* SunComponent = SunObject.AddComponent< Light >();
	SunComponent->SetDirection( Math::Normalize( Vector3::Down + Vector3::Right + Vector3::Forward * 0.3f ) );
	SunComponent->SetAmbient( Vector3( 1.0f, 0.3f, 0.3f ) );
	Light::SetSun( SunComponent );

	SunObject.AddComponent< OnLateGameplayUpdateComponent >()->Init( [=]()
	{
		static float i = 0;
		i += 3.0f * Time::GetDeltaTime();
		Light::GetSun()->SetDirection( Math::Normalize( Vector3( Math::Cos( i ), -1.0f, Math::Sin( i ) ) ) );
	} );

	static ResourceHandle< Material > FloorMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "FloorMaterial"_N );
		//Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "floor"_H ) );
		Temp.SetProperty( "diffuse_colour"_H, Vector4( 0.2f, 0.1f, 0.4f, 1.0f ) );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}( );

	static ResourceHandle< Prefab > FloorPrefab = []()
	{
		return Resource::Load< Prefab >( "plane"_H );
	}( );

	Vector2 LevelSize = Level::GetActiveLevel()->GetLevelSize();
	auto MidPoint = LevelSize / 2;

	for ( uint32_t Y = 0; Y < LevelSize.y; ++Y )
	{
		for ( uint32_t X = 0; X < LevelSize.x; ++X )
		{
			if ( Level::GetActiveLevel()->GetTile( { X, Y } ) == LevelObject::ABYSS )
			{
				continue;
			}

			GameObject NewFloor = Prefab::Instantiate( *FloorPrefab );
			MeshRenderer* NewRenderer = NewFloor.GetComponentInChild< MeshRenderer >();
			NewRenderer->SetMaterial( FloorMaterial );
			Transform* NewTransform = NewFloor.GetTransform();
			NewTransform->SetParent( a_Object, false );
			NewTransform->SetLocalPosition( Vector3{ X, 0u, Y } );
			NewTransform->SetLocalScale( Vector3::One );
		}
	}
	
};
void GraphicsPopulator::Abyss( GameObject a_Object ) { GraphicsPopulatorBase::Abyss( a_Object ); };
void GraphicsPopulator::Floor( GameObject a_Object ) { GraphicsPopulatorBase::Floor( a_Object ); };
void GraphicsPopulator::TentacleNode_( GameObject a_Object, TentacleNode* a_TentacleNode ) { GraphicsPopulatorBase::TentacleNode_( a_Object, a_TentacleNode ); };
void GraphicsPopulator::Random( GameObject a_Object ) { GraphicsPopulatorBase::Random( a_Object ); };
void GraphicsPopulator::Bonus( GameObject a_Object ) { GraphicsPopulatorBase::Bonus( a_Object ); };
void GraphicsPopulator::RandomBlocker( GameObject a_Object ) { GraphicsPopulatorBase::RandomBlocker( a_Object ); };


#define DefinePlayerMaterial( Name ) \
static ResourceHandle< Material > Name = []() \
	{ \
		Material Temp; \
		Temp.SetName( #Name##_N ); \
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( #Name##_H ) ); \
		Temp.SetShader( Shader::Diffuse ); \
		return Temp; \
	}() \


auto GetMaterialForDirection( Vector2Int a_Direction, uint32_t a_Frame )
{
	DefinePlayerMaterial( player_front_stand );
	DefinePlayerMaterial( player_front_walk_1 );
	DefinePlayerMaterial( player_front_walk_2 );
	DefinePlayerMaterial( player_back_stand );
	DefinePlayerMaterial( player_back_walk_1 );
	DefinePlayerMaterial( player_back_walk_2 );
	DefinePlayerMaterial( player_left_stand );
	DefinePlayerMaterial( player_left_walk_1 );
	DefinePlayerMaterial( player_left_walk_2 );
	DefinePlayerMaterial( player_right_stand );
	DefinePlayerMaterial( player_right_walk_1 );
	DefinePlayerMaterial( player_right_walk_2 );

	if ( a_Direction == Vector2Int::Down )
	{
		switch ( a_Frame )
		{
			case 0: return player_front_stand;
			case 1: return player_front_walk_1;
			case 2: return player_front_walk_2;
		}
	}
	else if ( a_Direction == Vector2Int::Up )
	{
		switch ( a_Frame )
		{
			case 0: return player_back_stand;
			case 1: return player_back_walk_1;
			case 2: return player_back_walk_2;
		}
	}
	else if ( a_Direction == Vector2Int::Left )
	{
		switch ( a_Frame )
		{
			case 0: return player_left_stand;
			case 1: return player_left_walk_1;
			case 2: return player_left_walk_2;
		}
	}
	else if ( a_Direction == Vector2Int::Right )
	{
		switch ( a_Frame )
		{
			case 0: return player_right_stand;
			case 1: return player_right_walk_1;
			case 2: return player_right_walk_2;
		}
	}
}

auto GetDirection( Vector3 Offset )
{
	if ( Offset.x < 0.0f )
	{
		return Vector2Int::Left;
	}
	else if ( Offset.x > 0.0f )
	{
		return Vector2Int::Right;
	}
	else if ( Offset.z > 0.0f )
	{
		return Vector2Int::Up;
	}
	else if ( Offset.z < 0.0f )
	{
		return Vector2Int::Down;
	}
	else
	{
		return Vector2Int::Down;
	}
};
//void GraphicsPopulator::Player( GameObject a_Object ) { GraphicsPopulatorBase::Player( a_Object ); }
void GraphicsPopulator::Player( GameObject a_Object ) 
{
	static ResourceHandle< Mesh > PlayerMesh = []()
	{
		return Resource::Load< Mesh >( "plane_mesh0"_H );
	}( );

	//auto plane = Resource::Load< Mesh >( "plane"_H );
	//GameObject NewPlayer = Prefab::Instantiate( *plane );
	GameObject NewPlayer = GameObject::Instantiate();
	//MeshRenderer* NewRenderer = NewPlayer.GetComponentInChild< MeshRenderer >();
	MeshRenderer* NewRenderer = NewPlayer.AddComponent< MeshRenderer >();
	NewRenderer->SetMaterial( GetMaterialForDirection( Vector2Int::Down, 0 ) );
	NewRenderer->SetMesh( PlayerMesh );
	NewRenderer->GetOwner()->GetTransform()->RotateGlobal( Vector3::Up * Math::Radians( 180.0f ) );

	Transform* NewTransform = NewPlayer.GetTransform();
	NewTransform->SetParent( a_Object, false );
	NewTransform->SetLocalScale( Vector3::One * 1.3f );
	NewTransform->TranslateLocalY( 0.6f );
	NewTransform->RotateLocal( Vector3::Right * Math::Radians( 20.0f ) );

	NewPlayer.AddComponent< OnLateGameplayUpdateComponent >()->Init( [NewPlayer, a_Object]()
	{
		auto Player = NewPlayer;
		auto Parent = a_Object;
		auto* PlayerComp = Parent.GetComponent< PlayerComponent >();
		Vector3 LastOffset = PlayerComp->GetLastMovementOffset();
		static const float TimeThatShouldBeSpentOnOneAnimationTile = 0.2f;
		auto Direction = GetDirection( LastOffset );
		Direction.x *= -1;
		//bool ChangedDirection = Direction != PlayerComp->PreviousDirection;
		PlayerComp->TimeSpentWalking += Time::GetRealDeltaTime();
		auto mat = GetMaterialForDirection( Direction, PlayerComp->AnimationState ? 0 : 2 );
		auto* meshrenderer = Player.GetComponent< MeshRenderer >();
		meshrenderer->SetMaterial( mat );
		if ( PlayerComp->TimeSpentWalking > TimeThatShouldBeSpentOnOneAnimationTile )
		{
			PlayerComp->TimeSpentWalking = 0.0f;
			PlayerComp->AnimationState = !PlayerComp->AnimationState;
		}

		PlayerComp->PreviousDirection = Direction;

	} );
};

void GraphicsPopulator::HighWall( GameObject a_Object )
{
	static ResourceHandle< Material > HighWallMaterial = []()
	{
		Material Temp;
		Temp.SetName( "HighWallMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "wall"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	static ResourceHandle< Mesh > HighWallCube = []()
	{
		return Resource::Load< Mesh >( "cube_mesh0"_H );
	}( );

	{
		GameObject SouthWall = GameObject::Instantiate();
		auto SouthWallTransform = SouthWall.GetTransform();
		auto SouthWallRenderer = SouthWall.AddComponent< MeshRenderer >();
		SouthWallRenderer->SetMesh( HighWallCube );
		SouthWallRenderer->SetMaterial( HighWallMaterial );
		//SouthWallTransform->RotateGlobal( Quaternion::ToQuaternion( Vector3{ Math::Radians( -90.0f ), Math::Radians( 0.0f ), 0.0f }, RotationOrder::XYZ ) );
		SouthWallTransform->SetParent( a_Object, false );
	}
	{
		GameObject SouthWall = GameObject::Instantiate();
		auto SouthWallTransform = SouthWall.GetTransform();
		auto SouthWallRenderer = SouthWall.AddComponent< MeshRenderer >();
		SouthWallRenderer->SetMesh( HighWallCube );
		SouthWallRenderer->SetMaterial( HighWallMaterial );
		//SouthWallTransform->RotateGlobal( Quaternion::ToQuaternion( Vector3{ Math::Radians( -90.0f ), Math::Radians( -90.0f ), 0.0f }, RotationOrder::XYZ ) );
		SouthWallTransform->GetTransform()->SetParent( a_Object, false );
		SouthWallTransform->SetLocalPositionY( 1.0f );
	}
};

void GraphicsPopulator::LowWall( GameObject a_Object )
{
	static ResourceHandle< Material > LowWallMaterial = []()
	{
		Material Temp;
		Temp.SetName( "LowWallMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "low_wall_top"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	static ResourceHandle< Mesh > LowWallCube = []()
	{
		return Resource::Load< Mesh >( "cube_mesh0"_H );
	}( );

	{
		GameObject SouthWall = GameObject::Instantiate();
		auto SouthWallTransform = SouthWall.GetTransform();
		auto SouthWallRenderer = SouthWall.AddComponent< MeshRenderer >();
		SouthWallRenderer->SetMesh( LowWallCube );
		SouthWallRenderer->SetMaterial( LowWallMaterial );
		//SouthWallTransform->RotateGlobal( Quaternion::ToQuaternion( Vector3{ Math::Radians( -90.0f ), Math::Radians( 0.0f ), 0.0f }, RotationOrder::XYZ ) );
		SouthWallTransform->SetParent( a_Object, false );
	}
};


//void GraphicsPopulator::Fuel( GameObject a_Object ) { GraphicsPopulatorBase::Fuel( a_Object ); };
void GraphicsPopulator::Fuel( GameObject a_Object )
{
	static ResourceHandle< Prefab > FuelPrefab = []()
	{
		return Resource::Load< Prefab >( "fuel"_H );
	}( );

	static ResourceHandle< Material > FuelMaterial = []()
	{
		Material Temp;
		Temp.SetName( "FuelMaterial"_N );
		Temp.AddProperty( "diffuse_colour"_N, ( Vector4 )Colour::LIGHT_BLUE );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}( );

	GameObject Fuel = Prefab::Instantiate( *FuelPrefab );
	Fuel.GetComponentInChild< MeshRenderer >()->SetMaterial( FuelMaterial );
	auto FuelTransform = Fuel.GetTransform();
	FuelTransform->SetParent( a_Object, false );
	FuelTransform->SetLocalScale( Vector3::One * 0.3f );
}


//void GraphicsPopulator::Generator( GameObject a_Object ) { GraphicsPopulatorBase::Generator( a_Object ); };
void GraphicsPopulator::Generator( GameObject a_Object )
{
	static ResourceHandle< Prefab > GeneratorPrefab = []()
	{
		return Resource::Load< Prefab >( "generator"_H );
	}( );

	static ResourceHandle< Material > GeneratorMaterial = []()
	{
		Material Temp;
		Temp.SetName( "GeneratorMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "generator_material0_texture_diffuse"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	//GameObject Fuel = Prefab::Instantiate( *GeneratorPrefab );
	////Fuel.GetComponentInChild< MeshRenderer >()->SetMaterial( FuelMaterial );
	//auto FuelTransform = Fuel.GetTransform();
	//FuelTransform->SetParent( a_Object, false );
	//FuelTransform->SetLocalScale( Vector3::One * 0.3f );

	ResourceHandle< Mesh > GeneratorMesh = Resource::Load< Mesh >( "generator_mesh1"_H );
	//ResourceHandle< Material > GeneratorMaterial = Resource::Load< Material >( "generator_material"_H );
	ResourceHandle< Texture2D > GeneratorTexture = Resource::Load< Texture2D >( "generator_material0_texture_diffuse"_H );

	GameObject NewGenerator = GameObject::Instantiate( a_Object );
	auto* NewGeneratorTransform = NewGenerator.GetTransform();
	auto* NewMeshRenderer = NewGenerator.AddComponent< MeshRenderer >();
	NewMeshRenderer->SetMaterial( GeneratorMaterial );
	NewMeshRenderer->SetMesh( GeneratorMesh );
	NewGeneratorTransform->SetParent( a_Object, false );
	NewGeneratorTransform->SetLocalRotation( Quaternion::ToQuaternion( Vector3::Up * Math::Radians( 0.0f ) ) );

};


//void GraphicsPopulator::TentacleStart( GameObject a_Object ) { GraphicsPopulatorBase::TentacleStart( a_Object ); };
void GraphicsPopulator::TentacleStart( GameObject a_Object )
{
	static ResourceHandle< Mesh > TentacleMesh = []()
	{
		return Resource::Load< Mesh >( "plane_mesh0"_H );
	}( );

	static ResourceHandle< Material > TentacleMaterialLeft = []()
	{
		Material Temp;
		Temp.SetName( "TentacleMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "tentacle_left"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	static ResourceHandle< Material > TentacleMaterialRight = []()
	{
		Material Temp;
		Temp.SetName( "TentacleMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "tentacle_right"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	GameObject NewTentacle = GameObject::Instantiate();
	MeshRenderer* NewRenderer = NewTentacle.AddComponent< MeshRenderer >();
	NewRenderer->SetMaterial( TentacleMaterialLeft );
	NewRenderer->SetMesh( TentacleMesh );
	NewRenderer->GetOwner()->GetTransform()->RotateGlobal( Vector3::Up * Math::Radians( 180.0f ) );

	Transform* NewTransform = NewTentacle.GetTransform();
	NewTransform->SetParent( a_Object, false );
	NewTransform->SetLocalScale( Vector3::One * 1.5f );
	NewTransform->TranslateLocalY( 0.4f );
	NewTransform->RotateLocal( Vector3::Right * Math::Radians( 60.0f ) );

	NewTentacle.AddComponent< OnLateGameplayUpdateComponent >()->Init( [NewTentacle, a_Object]()
	{
		auto Tentacle = NewTentacle;
		auto Parent = a_Object;
		auto* TentacleComp = Parent.GetComponent< TentacleComponent >();
		TentacleComp->TextureFlipTimer += Time::GetRealDeltaTime();
		static const float MaxTentacleFlipTime = 1.0f;

		if ( TentacleComp->TextureFlipTimer > MaxTentacleFlipTime )
		{
			TentacleComp->TextureFlipTimer = 0.0f;
			TentacleComp->TentacleOrientation = !TentacleComp->TentacleOrientation;
			Tentacle.GetComponent< MeshRenderer >()->SetMaterial( TentacleComp->TentacleOrientation ? TentacleMaterialLeft : TentacleMaterialRight );
		}
	} );
}

//void GraphicsPopulator::HorizontalExit( GameObject a_Object ) { GraphicsPopulatorBase::HorizontalExit( a_Object ); };
void GraphicsPopulator::HorizontalExit( GameObject a_Object )
{
	static ResourceHandle< Material > DoorOpenMaterial = []()
	{
		Material Temp;
		Temp.SetName( "DoorOpenMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "door_open"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	static ResourceHandle< Material > DoorClosedMaterial = []()
	{
		Material Temp;
		Temp.SetName( "DoorClosedMaterial"_N );
		Temp.AddTexture( "texture_diffuse"_N, Resource::Load< Texture2D >( "door_closed"_H ) );
		Temp.SetShader( Shader::Diffuse );
		return Temp;
	}( );

	static ResourceHandle< Material > DoorBack = []()
	{
		Material Temp;
		Temp.SetName( "DoorBack"_N );
		Temp.AddProperty( "diffuse_colour"_N, ( Vector4 )Colour::BLACK );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}( );

	static ResourceHandle< Prefab > DoorPrefab0 = []()
	{
		return Resource::Load< Prefab >( "cube"_H );
	}( );

	static ResourceHandle< Mesh > DoorMesh0 = []()
	{
		return Resource::Load< Mesh >( "plane_mesh0"_H );
	}( );

	GameObject NewDoor = Prefab::Instantiate( *DoorPrefab0 );
	auto* NewDoorTransform = NewDoor.GetTransform();
	NewDoorTransform->SetParent( a_Object, false );
	NewDoorTransform->SetLocalScale( Vector3( 4.0f, 3.0f, 1.1f ) );
	auto* NewDoorRenderer = NewDoor.GetComponentInChild< MeshRenderer >();
	NewDoorRenderer->SetMaterial( DoorBack );

	GameObject NewDoorFace = GameObject::Instantiate( a_Object );
	auto* NewDoorFaceTransform = NewDoorFace.GetTransform();
	NewDoorFaceTransform->RotateLocal( Vector3::Right * Math::Radians( 90.0f ) );
	NewDoorFaceTransform->SetLocalPositionZ( -0.6f );
	NewDoorFaceTransform->SetLocalScale( Vector3( 2.0f, 1.0f, 3.0f ) );
	auto* NewDoorFaceRenderer = NewDoorFace.AddComponent< MeshRenderer >();
	NewDoorFaceRenderer->SetMesh( DoorMesh0 );
	NewDoorFaceRenderer->SetMaterial( DoorOpenMaterial );
	
	NewDoor.AddComponent< OnFullyFueledChangedComponent >( )
	->Init([NewDoorFace]()
	{ 
		GameObject Door = NewDoorFace;

		if ( GeneratorSystem::IsFullyFueled() )
		{
			// Change material
			Door.GetComponent< MeshRenderer >()->SetMaterial( DoorOpenMaterial );
		}
		else
		{
			Door.GetComponent< MeshRenderer >()->SetMaterial( DoorClosedMaterial );
		}
	});
}