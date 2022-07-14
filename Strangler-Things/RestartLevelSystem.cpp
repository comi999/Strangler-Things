#include "Input.hpp"

#include "RestartLevelSystem.hpp"


bool RestartLevelSystem::Update()
{
	static bool WasRDown = false;

	// TODO: Make IsKeyPressed work!
	bool KeyPressedNow = Input::IsKeyDown( KeyCode::R );
	bool KeyJustPressed = KeyPressedNow && !WasRDown;
	WasRDown = KeyPressedNow;
	
	return KeyJustPressed;
}