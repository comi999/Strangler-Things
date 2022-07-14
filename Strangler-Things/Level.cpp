#include "Level.hpp"

#include "FuelGraphicsPopulator.hpp"
#include "GameplaySystem.hpp"
#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "GeneratorGraphicsPopulator.hpp"
#include "HighWallGraphicsPopulator.hpp"
#include "HorizontalExitGraphicsPopulator.hpp"
#include "LowWallGraphicsPopulator.hpp"
#include "PickUpAbleComponent.hpp"
#include "PlayerComponent.hpp"
#include "PlayerGraphicsPopulator.hpp"
#include "SceneGraphicsPopulator.hpp"
#include "CameraSystem.hpp"
#include "Atlas.hpp"
#include "SfxrClip.hpp"
#include "AudioSource.hpp"

std::map< Hash, Action< GameObject& > > GraphicsPopulators =
{
	std::make_pair( "HighWall"_H, Action< GameObject& >( new HighWallGraphicsPopulator(), &HighWallGraphicsPopulator::Populate ) ),
	std::make_pair( "LowWall"_H, Action< GameObject& >( new LowWallGraphicsPopulator(), &LowWallGraphicsPopulator::Populate ) ),
	std::make_pair( "Player"_H, Action< GameObject& >( new PlayerGraphicsPopulator(), &PlayerGraphicsPopulator::Populate ) ),
	std::make_pair( "HorizontalExit"_H, Action< GameObject& >( new HorizontalExitGraphicsPopulator(), &HorizontalExitGraphicsPopulator::Populate ) ),
	std::make_pair( "Fuel"_H, Action< GameObject& >( new FuelGraphicsPopulator(), &FuelGraphicsPopulator::Populate ) ),
	std::make_pair( "Generator"_H, Action< GameObject& >( new GeneratorGraphicsPopulator(), &GeneratorGraphicsPopulator::Populate ) )
};

template < Hash _ObjectName >
GameObject CreateMapObject( Vector3Int a_Coord )
{
	GameObject NewMapObj = GameObject::Instantiate();
	NewMapObj.GetTransform()->SetGlobalPosition( a_Coord );
	GraphicsPopulators[ _ObjectName ]( NewMapObj );
	return NewMapObj;
}

template < Hash _ObjectName >
GameObject CreateMapObjectWithGridOccupancy( Vector3Int a_Coord )
{
	//GameplaySystem::GetMatch().lock()->GetGridOccupancySystem().RegisterOccupied( a_Coord );
	Level::GetActiveLevel()->GetAtlas().DisableNode( a_Coord.swizzle.xz );
	return CreateMapObject< _ObjectName >( a_Coord );
}

GameObject CreatePlayer( Vector3Int a_Coord )
{
	GameObject Player = CreateMapObject< "Player"_H >( a_Coord );
	Player.AddComponent< PlayerComponent >();
	auto* audioSource = Player.AddComponent< AudioSource >();
	auto footstepSfx = Resource::Load< SfxrClip >( "footstep"_H );
	audioSource->LoadSfx( footstepSfx );
	audioSource->SetRolloffFactor( 0.1f );

	CameraSystem::Follow( Player );

	return Player;
}

GameObject CreateHorizontalExit( Vector3Int a_Coord )
{
	GameObject Exit = CreateMapObject< "HorizontalExit"_H >( a_Coord );
	Exit.AddComponent< AudioSource >();

	return Exit;
}

GameObject CreateFuel( Vector3Int a_Coord )
{
	GameObject Fuel = CreateMapObject< "Fuel"_H >( a_Coord );
	Fuel.AddComponent< PickUpAbleComponent >();
	Fuel.AddComponent< GeneratorFuelComponent >();
	Fuel.AddComponent< AudioSource >();

	return Fuel;
}

GameObject CreateGenerator( Vector3Int a_Coord )
{
	GameObject Generator = CreateMapObject< "Generator"_H >( a_Coord );
	Generator.AddComponent< GeneratorComponent >();
	auto* audioSource = Generator.AddComponent< AudioSource >();
	auto sound = Resource::Load< SfxrClip >( "powerup"_H );
	audioSource->LoadSfx( sound );
	audioSource->SetRolloffFactor( 0.1f );

	return Generator;
}

std::map< LevelObject, Invoker< GameObject, Vector3Int > > TileCallbacks =
{
	std::make_pair( LevelObject::FLOOR,          Invoker< GameObject, Vector3Int >() ),
	std::make_pair( LevelObject::ABYSS,          Invoker< GameObject, Vector3Int >() ),
	std::make_pair( LevelObject::HIGH_WALL ,     CreateMapObjectWithGridOccupancy< "HighWall"_H  > ),
	std::make_pair( LevelObject::LOW_WALL,       CreateMapObjectWithGridOccupancy< "LowWall"_H > ),
	std::make_pair( LevelObject::PLAYER_START,   CreatePlayer ),
	std::make_pair( LevelObject::EXIT,           CreateHorizontalExit ),
	std::make_pair( LevelObject::TENTACLE_START, Invoker< GameObject, Vector3Int >() ),
	std::make_pair( LevelObject::RANDOM,         Invoker< GameObject, Vector3Int >() ),
	std::make_pair( LevelObject::FUEL,           CreateFuel ),
	std::make_pair( LevelObject::GENERATOR,      CreateGenerator ),
	std::make_pair( LevelObject::BONUS,          Invoker< GameObject, Vector3Int >() )
};


Level::Level( const std::string& a_Tilemap, Vector2UInt a_Dimensions )
	: m_LevelAtlas( a_Dimensions )
	, m_LevelSize( a_Dimensions )
	, m_LevelData( a_Tilemap )
{}

Vector2UInt Level::GetLevelSize() const
{
	return m_LevelSize;
}

LevelObject Level::GetLevelObject( Vector2UInt a_Coordinate ) const
{
	size_t Offset = ( size_t )a_Coordinate.y * m_LevelSize.x + a_Coordinate.x;

	if ( Offset >= m_LevelData.size() )
	{
		return LevelObject::ABYSS;
	}

	return ( LevelObject )m_LevelData[ Offset ];
}

void Level::PopulateScene( GameObject a_Scene )
{
	Atlas& at = s_ActiveLevel->m_LevelAtlas;
	for ( int32_t Y = 0; Y < m_LevelSize.y; ++Y )
	{
		for ( int32_t X = 0; X < m_LevelSize.x; ++X )
		{
			LevelObject ObjectAtLocation = GetLevelObject( Vector2UInt{ X, Y } );
			auto Callback = TileCallbacks.find( ObjectAtLocation );

			if ( Callback != TileCallbacks.end() )
			{
				GameObject NewObject = Callback->second( Vector3Int( X, 0, Y ) );
				if ( NewObject.IsValid() ) a_Scene.GetTransform()->AttachChild( NewObject );
			}
		}
	}
}

void Level::ResetAtlas()
{
	m_LevelAtlas = Atlas( m_LevelSize );
}

Atlas& Level::GetAtlas()
{
	return m_LevelAtlas;
}

const Atlas& Level::GetAtlas() const
{
	return m_LevelAtlas;
}

Level* Level::CreateNewLevel( Hash a_Name, const std::string& a_Tilemap, Vector2UInt a_Size )
{
	auto Result = s_Levels.emplace( std::piecewise_construct, std::forward_as_tuple( a_Name ), std::forward_as_tuple( a_Tilemap, a_Size ) );
	return Result.second ? &Result.first->second : nullptr;
}

Level* Level::GetLevel( Hash a_Name )
{
	auto Result = s_Levels.find( a_Name );
	return Result != s_Levels.end() ? &Result->second : nullptr;
}

Level* Level::GetActiveLevel()
{
	return s_ActiveLevel;
}

bool Level::SetActiveLevel( Hash a_Name )
{
	auto Iter = s_Levels.find( a_Name );

	if ( Iter == s_Levels.end() )
	{
		return false;
	}

	s_ActiveLevel = &Iter->second;
	return true;
}