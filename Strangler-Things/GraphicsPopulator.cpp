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
};
void GraphicsPopulator::Abyss( GameObject a_Object ) { GraphicsPopulatorBase::Abyss( a_Object ); };
void GraphicsPopulator::Floor( GameObject a_Object ) { GraphicsPopulatorBase::Floor( a_Object ); };
void GraphicsPopulator::HorizontalExit( GameObject a_Object ) { GraphicsPopulatorBase::HorizontalExit( a_Object ); };
void GraphicsPopulator::TentacleStart( GameObject a_Object ) { GraphicsPopulatorBase::TentacleStart( a_Object ); };
void GraphicsPopulator::TentacleNode_( GameObject a_Object, TentacleNode* a_TentacleNode ) { GraphicsPopulatorBase::TentacleNode_( a_Object, a_TentacleNode ); };
void GraphicsPopulator::Random( GameObject a_Object ) { GraphicsPopulatorBase::Random( a_Object ); };
void GraphicsPopulator::Generator( GameObject a_Object ) { GraphicsPopulatorBase::Generator( a_Object ); };
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
	NewTransform->SetLocalScale( Vector3::One * 1.5f );
	NewTransform->TranslateLocalY( 0.1f );

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

	GameObject Fuel = Prefab::Instantiate( *FuelPrefab );
	auto FuelTransform = Fuel.GetTransform();
	FuelTransform->SetParent( a_Object, false );
	FuelTransform->SetLocalScale( Vector3::One * 0.3f );
}