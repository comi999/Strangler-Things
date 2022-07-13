#pragma once
#include <sstream>
#include <iostream>

#include "File.hpp"
#include "GameObject.hpp"
#include "Math.hpp"

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

enum class LevelObject : uint8_t
{
	ABYSS          = '.',
	FLOOR          = ' ',
	HIGH_WALL      = '#',
	LOW_WALL       = '-',
	EXIT           = '_',
	PLAYER_START   = 'p',
	TENTACLE_START = 't',
	RANDOM         = '?',
	FUEL           = 'f',
	GENERATOR      = 'g',
	BONUS          = 's'
};

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
	GameplaySystem::GetMatch().lock()->GetGridOccupancySystem().RegisterOccupied( a_Coord );
	return CreateMapObject< _ObjectName >( a_Coord );
}

GameObject CreatePlayer( Vector3Int a_Coord )
{
	GameObject Player = CreateMapObject< "Player"_H >( a_Coord );
	Player.AddComponent< PlayerComponent >();

	CameraSystem::Follow( Player );

	return Player;
}

GameObject CreateHorizontalExit( Vector3Int a_Coord )
{
	GameObject Exit = CreateMapObject< "HorizontalExit"_H >( a_Coord );

	return Exit;
}

GameObject CreateFuel( Vector3Int a_Coord )
{
	GameObject Fuel = CreateMapObject< "Fuel"_H >( a_Coord );
	Fuel.AddComponent< PickUpAbleComponent >();
	Fuel.AddComponent< GeneratorFuelComponent >();

	return Fuel;
}

GameObject CreateGenerator( Vector3Int a_Coord )
{
	GameObject Generator = CreateMapObject< "Generator"_H >( a_Coord );
	Generator.AddComponent< GeneratorComponent >();

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
	std::make_pair( LevelObject::GENERATOR,      CreateGenerator),
	std::make_pair( LevelObject::BONUS,          Invoker< GameObject, Vector3Int >() )
};



class Level
{
public:

	Level( const std::string& a_Tilemap, Vector2UInt a_Dimensions )
		: m_LevelAtlas( a_Dimensions )
		, m_LevelSize( a_Dimensions )
		, m_LevelData( a_Tilemap )
	{ }

	inline Vector2UInt GetLevelSize() const
	{
		return m_LevelSize;
	}

	LevelObject GetLevelObject( Vector2UInt a_Coordinate ) const
	{
		size_t Offset = ( size_t )a_Coordinate.y * m_LevelSize.x + a_Coordinate.x;
		
		if ( Offset >= m_LevelData.size() )
		{
			return LevelObject::ABYSS;
		}

		return ( LevelObject )m_LevelData[ Offset ];
	}

	void PopulateScene( GameObject a_Scene )
	{
		for ( int32_t Y = 0; Y < m_LevelSize.y; ++Y )
		{
			for ( int32_t X = 0; X < m_LevelSize.x; ++X )
			{
				LevelObject ObjectAtLocation = GetLevelObject( Vector2UInt{ X, Y } );
				auto Callback = TileCallbacks.find( ObjectAtLocation );

				if ( Callback != TileCallbacks.end() )
				{
					GameObject NewObject = Callback->second( Vector3Int( X, 0, -Y ) );
					if ( NewObject.IsValid() ) a_Scene.GetTransform()->AttachChild( NewObject );
				}
			}
		}
	}

private:

	Vector2UInt m_LevelSize;
	std::string m_LevelData;
	Atlas       m_LevelAtlas;
};