#include "Level.hpp"
#include "Atlas.hpp"
#include "AudioPopulator.hpp"
#include "GameplaySystem.hpp"
#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "GraphicsPopulator.hpp"
#include "LogicPopulator.hpp"
#include "PickUpAbleComponent.hpp"
#include "PlayerComponent.hpp"
#include "CameraSystem.hpp"
#include "MeshRenderer.hpp"

GameObject LevelGameObject;

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

GameObject CreateTileGameObject( LevelObject a_ObjectType, Vector3Int a_Coord )
{
	GameObject NewMapObj = GameObject::Instantiate();
	NewMapObj.GetTransform()->SetGlobalPosition( a_Coord );
	std::get< 0 >( Populators[ a_ObjectType ] )( NewMapObj );
	std::get< 1 >( Populators[ a_ObjectType ] )( NewMapObj );
	std::get< 2 >( Populators[ a_ObjectType ] )( NewMapObj );
	return NewMapObj;
}

Level::Level( Hash a_Hash, const std::string& a_Tilemap, Vector2UInt a_Dimensions )
	: m_Name( a_Hash )
	, m_LevelAtlas( a_Dimensions )
	, m_LevelSize( a_Dimensions )
	, m_LevelData( a_Tilemap )
{}

Vector2UInt Level::GetLevelSize() const
{
	return m_LevelSize;
}

LevelObject Level::GetTile( Vector2UInt a_Coordinate ) const
{
	size_t Offset = ( size_t )a_Coordinate.y * m_LevelSize.x + a_Coordinate.x;

	if ( Offset >= m_LevelData.size() )
	{
		return LevelObject::ABYSS;
	}

	return ( LevelObject )m_LevelData[ Offset ];
}

std::string FileToString( File& a_File )
{
	if ( a_File.Open() )
	{
		std::string S;
		S.resize( a_File.Size() );
		a_File.Read( S.data(), S.size() );
		a_File.Close();

		return S;
	}

	throw std::exception( "Could not open file" );
}

void Level::CreateNewLevel( Hash a_Name, std::string a_LevelPath )
{
	std::string TilemapString = "..\n..";
	Vector2UInt LevelSize = Vector2UInt( 2, 2 );

	if ( a_LevelPath != "" )
	{
		File LevelFile = Path( a_LevelPath.c_str() );
		TilemapString = FileToString( LevelFile );

		std::stringstream TilemapStream( TilemapString );
		std::vector< std::string > Rows;
		size_t Width = 0;
	
		// Create collection of rows and store largest Width.
		while ( !TilemapStream.eof() )
		{
			auto& NewRow = Rows.emplace_back();
			std::getline( TilemapStream, NewRow );
			Width = Math::Max( Width, NewRow.size() );
		}
	
		// Pad out all rows to Width.
		for ( auto& Row : Rows )
		{
			Row.resize( Width );
		}
	
		// Get the size of the level.
		LevelSize = { Width, Rows.size() };
	
		// Compile all rows into level data.
		TilemapString.clear();
	
		for ( auto Begin = Rows.rbegin(), End = Rows.rend(); Begin != End; ++Begin )
		{
			TilemapString += *Begin;
		}
	}

	s_Levels.emplace( std::piecewise_construct, std::forward_as_tuple( a_Name ), std::forward_as_tuple( a_Name, TilemapString, LevelSize ) );
}

void Level::Preload( std::vector< std::pair< Hash, std::string > > a_Levels )
{
	for ( auto& LevelPair : a_Levels )
	{
		CreateNewLevel( LevelPair.first, LevelPair.second );
	}
}

bool Level::SetActiveLevel( Hash a_Name )
{
	auto Iter = s_Levels.find( a_Name );

	if ( Iter == s_Levels.end() )
	{
		return false;
	}
	
	if ( s_ActiveLevel != nullptr )
	{
		// Things will probably go wrong here! It's meant to be deactivated in advance via GameplaySystem
		s_ActiveLevel->Deactivate();
	}

	LevelGameObject = GameObject::Instantiate( "Level"_N );

	s_ActiveLevel = &Iter->second;
	s_ActiveLevel->ResetAtlas();

	GP.Scene( LevelGameObject, a_Name );
	AP_.Scene( LevelGameObject, a_Name );

	for ( int32_t Y = 0; Y < s_ActiveLevel->m_LevelSize.y; ++Y )
	{
		for ( int32_t X = 0; X < s_ActiveLevel->m_LevelSize.x; ++X )
		{
			LevelObject ObjectAtLocation = s_ActiveLevel->GetTile( Vector2UInt{ X, Y } );

			if ( Populators.find( ObjectAtLocation ) == Populators.end() )
			{
				continue;
			}

			GameObject NewObject = CreateTileGameObject( ObjectAtLocation, Vector3Int( X, 0, Y ) );
			if ( NewObject.IsValid() ) LevelGameObject.GetTransform()->AttachChild( NewObject );
		}
	}

	return true;
}

void Level::Deactivate()
{
	if ( LevelGameObject.IsValid() )
	{
		GameObject::Destroy( LevelGameObject );
	}

	s_ActiveLevel = nullptr;
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

const Hash Level::GetName() const
{
	return m_Name;
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
