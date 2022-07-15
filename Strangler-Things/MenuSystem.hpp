#pragma once

#include "SplashScreenController.hpp"
#include "MainMenuController.hpp"

class MenuSystem
{
public:

	MenuSystem()
	{
		MenuObject = GameObject::Instantiate();
	}

	void Update()
	{
		static float Progress = 0.0f;
		//if ( !Active ) return;

		if ( Reset )
		{
			Reset = false;
			Progress = 0.0f;

			if ( MenuObject.IsValid() )
			{
				GameObject::Destroy( MenuObject );
			}

			MenuObject = GameObject::Instantiate();
		}

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
	}

	bool Active = false;
	bool Reset = false;

private:

	GameObject MenuObject;
};

static MenuSystem _MS;