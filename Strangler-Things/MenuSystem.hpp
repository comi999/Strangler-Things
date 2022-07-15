#pragma once

#include "Input.hpp"

#include "GameplaySystem.hpp"
#include "SplashScreenController.hpp"
#include "MainMenuController.hpp"

class MenuSystem
{
public:

	static MenuSystem& I()
	{
		if ( s_I == nullptr )
		{
			s_I = new MenuSystem;
		}

		return *s_I;
	}

	MenuSystem()
	{
		MenuObject = GameObject::Instantiate();
	}

	void Update()
	{
		static float Progress = 0.0f;


		if ( Reset )
		{
			Progress = 0.0f;

			if ( MenuObject.IsValid() )
			{
				GameObject::Destroy( MenuObject );
			}

			//MenuObject = GameObject::Instantiate();
			return;
		}

		
		if ( !Active ) return;

		Progress += Time::GetRealDeltaTime();

		if ( Progress < 5.0f )
		{
			SplashScreenController* SplashScreen = MenuObject.GetComponent< SplashScreenController >();

			if ( !SplashScreen )
			{
				SplashScreen = MenuObject.GetComponent< SplashScreenController >();
				SplashScreen->Init( MenuObject );
			}

			SplashScreen->TickSplash( Progress - 0.0f );
			return;
		}

		if ( MenuObject.IsValid() )
		{
			GameObject::Destroy( MenuObject );
			MenuObject = GameObject::Instantiate();
		}

		auto* MenuController = MenuObject.GetComponent< MainMenuController >();

		if ( !MenuController )
		{
			MenuController = MenuObject.AddComponent< MainMenuController >();
			MenuController->Init( MenuObject );
		}

		MenuController->Tick( Progress );

		if (
			Input::IsKeyDown( KeyCode::Space ) ||
			Input::IsKeyDown( KeyCode::Enter )
		)
		{
			Reset = true;
			 GameplaySystem::StartLevel(
				GameplaySystem::GetOrderedLevels().front()
			);
		}
	}

	bool Active = false;
	bool Reset = false;

private:
	inline static MenuSystem* s_I = nullptr;

	GameObject MenuObject;
};