#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "File.hpp"
#include "GameObject.hpp"
#include "Invoker.hpp"
#include "Math.hpp"
#include "AudioSource.hpp"

#include "CameraSystem.hpp"
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

#include <iostream>


GameObject SceneObj;

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
	GameObject NewMapObj = GameObject::Instantiate( SceneObj );
	// TODO: Changing from Local to Global position here causes everything spawned to be located at origin?!?
	// (Not important for this game, but just a strange bug for the Lengine)
	//NewMapObj.GetTransform()->SetLocalPosition( a_Coord );
	NewMapObj.GetTransform()->SetGlobalPosition( a_Coord );

	Vector3 NewMapObjGlobalPos = NewMapObj.GetTransform()->GetGlobalPosition();

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
	auto* audioSource = Player.AddComponent< AudioSource >();
	auto footstepSfx = Resource::Load< SfxrClip >("footstep"_H);
	audioSource->LoadSfx(footstepSfx);
	audioSource->SetRolloffFactor(0.1f);

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
	auto sound = Resource::Load< SfxrClip >("powerup"_H);
	audioSource->LoadSfx(sound);
	audioSource->SetRolloffFactor(0.1f);

	return Generator;
}

std::map< char, Invoker< GameObject, Vector3Int > > TileCallbacks =
{
	std::make_pair( ' ', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( '.', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( '#', CreateMapObjectWithGridOccupancy< "HighWall"_H  > ),
	std::make_pair( '-', CreateMapObjectWithGridOccupancy< "LowWall"_H > ),
	std::make_pair( 'p', CreatePlayer ),
	std::make_pair( '_', CreateHorizontalExit ),
	std::make_pair( 't', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( '?', Invoker< GameObject, Vector3Int >() ),
	std::make_pair( 'f', CreateFuel ),
	std::make_pair( 'g', CreateGenerator ),
	std::make_pair( 's', Invoker< GameObject, Vector3Int >() )
};

std::string FileToString( const Path& a_FilePath )
{
	File F = a_FilePath;
	if ( F.Open() )
	{
		std::string S;
		S.resize( F.Size() );
		F.Read( S.data(), S.size() );

		// TODO: Close on destructor?
		F.Close();

		return S;
	}

	throw std::exception( "Could not open file" );
}

void LoadScene( const Path& a_TilemapPath )
{
	_STL_ASSERT( !SceneObj.IsValid(), "Scene unloading not yet supported" );
	_STL_ASSERT( a_TilemapPath.IsFile(), "TilemapPath isn't pointing to a file" );

	SceneObj = GameObject::Instantiate( "Scene"_N );

	SceneGraphicsPopulator().Populate( SceneObj );

	std::string Tilemap = FileToString( a_TilemapPath );
	std::stringstream TilemapLines( Tilemap );

	int Y = 0;
	while ( !TilemapLines.eof() )
	{
		int X = 0;

		std::string Line;
		std::getline( TilemapLines, Line );

		for ( auto Begin = Line.begin(), End = Line.end(); Begin != End; ++Begin, ++X )
		{
			if ( *Begin == '\r' )
			{
				continue;
			}

			auto Iter = TileCallbacks.find( *Begin );
			_STL_ASSERT(
				Iter != TileCallbacks.end(),
				( "Unsupported tile found in tilemap: " + std::to_string( *Begin ) + " (" + std::to_string( X ) + ", " + std::to_string ( Y ) + ") " ).c_str()
			);

			if ( Iter != TileCallbacks.end() )
			{
				Iter->second( Vector3Int( X, 0, -Y ) );
			}
		}

		++Y;
	}
}