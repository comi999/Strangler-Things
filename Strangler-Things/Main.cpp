#include <iostream>

#include "CGE.hpp"
#include "File.hpp"
#include "Invoker.hpp"

#include "CameraSystem.hpp"
#include "GraphicsPopulator.hpp"
#include "SceneLoader.hpp"


int main( int argc, char** argv )
{
	_STL_ASSERT( argc >= 2, "Missing tilemap path argument" );

	auto Window = ConsoleWindow::Create( "Strangler Things", { 150, 120 }, { 8, 8 } );
	ConsoleWindow::MakeContextCurrent( Window );
	CGE::Init();

	CameraSystem CameraSystem;

	Path TestTilemap = Path( argv[1] );
	LoadScene( TestTilemap );

	Action<> GameLoop = [&]()
	{
		CameraSystem.Update();
	};

	CGE::Run( GameLoop );

	return 0;
}