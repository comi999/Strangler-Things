#include "Level.hpp"
#include "Atlas.hpp"
#include "AudioPopulator.hpp"
#include "GameplaySystem.hpp"
#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "GraphicsPopulator.hpp"
#include "LogicPopulator.hpp"
#include "OnExitStatusChangedComponent.hpp"
#include "PickUpAbleComponent.hpp"
#include "PlayerComponent.hpp"
#include "CameraSystem.hpp"


std::map< LevelObject, std::tuple< Action< GameObject >, Action< GameObject >, Action< GameObject > > > Populators = 
{
	std::make_pair( LevelObject::ABYSS,           std::make_tuple( Action< GameObject >(),                                       Action< GameObject >( GP, &GraphicsPopulator::Abyss ),          Action< GameObject >()                                       ) ),
	std::make_pair( LevelObject::FLOOR,           std::make_tuple( Action< GameObject >(),                                       Action< GameObject >( GP, &GraphicsPopulator::Floor ),          Action< GameObject >()                                       ) ),
	std::make_pair( LevelObject::HIGH_WALL,       std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::HighWall ),       Action< GameObject >( GP, &GraphicsPopulator::HighWall ),       Action< GameObject >()                                       ) ),
	std::make_pair( LevelObject::LOW_WALL,        std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::LowWall ),        Action< GameObject >( GP, &GraphicsPopulator::LowWall ),        Action< GameObject >()                                       ) ),
	std::make_pair( LevelObject::RANDOM_BLOCKER,  std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::RandomBlocker ),  Action< GameObject >( GP, &GraphicsPopulator::RandomBlocker ),  Action< GameObject >()                                       ) ),
	std::make_pair( LevelObject::PLAYER,          std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::Player ),         Action< GameObject >( GP, &GraphicsPopulator::Player ),         Action< GameObject >( AP_, &AudioPopulator::Player )         ) ),
	std::make_pair( LevelObject::HORIZONTAL_EXIT, std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::HorizontalExit ), Action< GameObject >( GP, &GraphicsPopulator::HorizontalExit ), Action< GameObject >( AP_, &AudioPopulator::HorizontalExit ) ) ),
	std::make_pair( LevelObject::FUEL,            std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::Fuel ),           Action< GameObject >( GP, &GraphicsPopulator::Fuel ),           Action< GameObject >( AP_, &AudioPopulator::Fuel )           ) ),
	std::make_pair( LevelObject::GENERATOR,       std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::Generator ),      Action< GameObject >( GP, &GraphicsPopulator::Generator ),      Action< GameObject >( AP_, &AudioPopulator::Generator )      ) ),
	std::make_pair( LevelObject::TENTACLE_START,  std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::TentacleStart ),  Action< GameObject >( GP, &GraphicsPopulator::TentacleStart ),  Action< GameObject >( AP_, &AudioPopulator::TentacleStart )  ) ),
	std::make_pair( LevelObject::BONUS,           std::make_tuple( Action< GameObject >( LP_, &LogicPopulator::Bonus ),          Action< GameObject >( GP, &GraphicsPopulator::Bonus ),          Action< GameObject >( AP_, &AudioPopulator::Bonus )          ) ),
};

GameObject CreateMapObject( LevelObject a_ObjectType, Vector3Int a_Coord )
{
	GameObject NewMapObj = GameObject::Instantiate();
	NewMapObj.GetTransform()->SetGlobalPosition( a_Coord );
	std::get< 0 >( Populators[ a_ObjectType ] )( NewMapObj );
	std::get< 1 >( Populators[ a_ObjectType ] )( NewMapObj );
	std::get< 2 >( Populators[ a_ObjectType ] )( NewMapObj );
	return NewMapObj;
}

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
	GP.Scene( a_Scene );
	AP_.Scene( a_Scene );

	Atlas& at = s_ActiveLevel->m_LevelAtlas;
	for ( int32_t Y = 0; Y < m_LevelSize.y; ++Y )
	{
		for ( int32_t X = 0; X < m_LevelSize.x; ++X )
		{
			LevelObject ObjectAtLocation = GetLevelObject( Vector2UInt{ X, Y } );

			if ( Populators.find( ObjectAtLocation ) == Populators.end() )
			{
				continue;
			}

			GameObject NewObject = CreateMapObject( ObjectAtLocation, Vector3Int( X, 0, Y ) );
			if ( NewObject.IsValid() ) a_Scene.GetTransform()->AttachChild( NewObject );
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