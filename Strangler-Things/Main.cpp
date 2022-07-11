#include <iostream>

#include "CGE.hpp"
#include "File.hpp"
#include "Invoker.hpp"

#include "CameraSystem.hpp"
//#include "GraphicsPopulator.hpp"
#include "SceneLoader.hpp"


int main( int argc, char** argv )
{
	auto Window = ConsoleWindow::Create( "Strangler Things", { 64, 64 }, { 8, 8 } );
	ConsoleWindow::MakeContextCurrent( Window );
	CGE::Init();

	CameraSystem CameraSystem;

	Path TilemapLevels[] {
		Path( "./Resources/Maps/Level1.txt" ),
		Path( "./Resources/Maps/Level2.txt" ),
		Path( "./Resources/Maps/Level3.txt" )
	};
	
	/*
	Path StartingMap = (argc >= 2) ? Path( argv[1] ) : TilemapLevels[0];
	*/
	// TODO: Using a ternary here causes a crash?!
	Path StartingMap;
	if (argc >= 2)
		StartingMap = Path( argv[1] );
	else StartingMap = TilemapLevels[0];
	
	LoadScene( StartingMap );

	Action<> GameLoop = [&]()
	{
		CameraSystem.Update();
	};

	CGE::Run( GameLoop );

	return 0;
}