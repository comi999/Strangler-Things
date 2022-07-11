#include <iostream>

#include "CGE.hpp"
#include "File.hpp"
#include "Invoker.hpp"

#include "CameraSystem.hpp"
#include "GraphicsPopulator.hpp"
#include "SceneLoader.hpp"


int main()
{
	auto Window = ConsoleWindow::Create( "Strangler Things", { 64, 64 }, { 8, 8 } );
	ConsoleWindow::MakeContextCurrent( Window );
	CGE::Init();

	CameraSystem CameraSystem;

	_STL_ASSERT( Directory{}.ContainsFile( "./Resources/Maps/Map.txt" ), "Missing tilemap" );
	Path TestTilemap( "./Resources/Maps/Map.txt" );
	
	LoadScene( TestTilemap );

	Action<> GameLoop = [&]()
	{
		CameraSystem.Update();
	};

	CGE::Run( GameLoop );

	return 0;
}