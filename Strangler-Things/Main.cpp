#include <iostream>

#include "CGE.hpp"
#include "File.hpp"
#include "Invoker.hpp"

#include "CameraSystem.hpp"
//#include "GraphicsPopulator.hpp"
#include "SceneLoader.hpp"


int main()
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
	
	LoadScene( TilemapLevels[ 0 ] );

	Action<> GameLoop = [&]()
	{
		CameraSystem.Update();
	};

	CGE::Run( GameLoop );

	return 0;
}