#include <vector>

#include "CGE.hpp"
#include "File.hpp"
#include "Invoker.hpp"

#include "AudioPopulator.hpp"
#include "CameraSystem.hpp"
#include "Level.hpp"
#include "GameplaySystem.hpp"
#include "MenuSystem.hpp"


int main( int argc, char** argv )
{
	auto Window = ConsoleWindow::Create( "Strangler Things", { 128, 96 }, { 8, 8 } );
	ConsoleWindow::MakeContextCurrent( Window );
	CGE::Init();

	CameraSystem CameraSystem;
	GameplaySystem GameplaySystem{};
	MenuSystem MenuSystem{};

	Hash StartingLevel = "Menu"_H;
	std::vector< std::pair< Hash, std::string > > Levels {
		std::make_pair( "Menu"_H,   ""                              ),
		std::make_pair( "Level1"_H, "./Resources/Levels/Level1.txt" ),
		std::make_pair( "Level2"_H, "./Resources/Levels/Level2.txt" ),
		std::make_pair( "Level3"_H, "./Resources/Levels/Level3.txt" )
	};

	if ( argc >= 2 )
	{
		Levels.push_back( std::make_pair( "CLILevel"_H, Path( argv[1] ) ) );
		StartingLevel = "CLILevel"_H;
	}

	Level::Preload( Levels );
	
	AP_.Global();
	GameplaySystem::StartLevel( StartingLevel );

	Action<> GameLoop = [&]()
	{
		_MS.Update();
		GameplaySystem.Update();
		CameraSystem.Update();
	};

	CGE::Run( GameLoop );

	return 0;
}