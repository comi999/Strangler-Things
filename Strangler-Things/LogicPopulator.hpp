#pragma once

#include "Atlas.hpp"
#include "CameraSystem.hpp"
#include "ExitComponent.hpp"
#include "GameplaySystem.hpp"
#include "GameObject.hpp"
#include "GeneratorComponent.hpp"
#include "GeneratorFuelComponent.hpp"
#include "OnFullyFueledChangedComponent.hpp"
#include "PickUpAbleComponent.hpp"
#include "PlayerComponent.hpp"
#include "TentacleComponent.hpp"


class LogicPopulator
{
public:
	void Scene( GameObject a_Object ) { };
	void HorizontalExit( GameObject a_Object )
	{
		a_Object.AddComponent< ExitComponent >();

		Vector3Int Coord = a_Object.GetTransform()->GetGlobalPosition();

		GameObject::Instantiate( a_Object )
			.AddComponent< OnFullyFueledChangedComponent >( )
			->Init([=]()
			{
				if ( GeneratorSystem::IsFullyFueled() )
				{
					Level::GetActiveLevel()->GetAtlas().EnableNode( Coord.swizzle.xz );
				}
				else
				{
					Level::GetActiveLevel()->GetAtlas().DisableNode( Coord.swizzle.xz );
				}
			});
	};
	void Player( GameObject a_Object )
	{
		a_Object.AddComponent< PlayerComponent >();
		CameraSystem::Follow( a_Object );

	};
	void TentacleStart( GameObject a_Object )
	{
		a_Object.AddComponent< TentacleComponent >()->Init();
		AddCollision( a_Object );
	};
	void Random( GameObject a_Object ) { };
	void Fuel( GameObject a_Object )
	{
		a_Object.AddComponent< PickUpAbleComponent >();
		a_Object.AddComponent< GeneratorFuelComponent >();
	};
	void Generator( GameObject a_Object )
	{
		a_Object.AddComponent< GeneratorComponent >();
	};
	void Bonus( GameObject a_Object ) { };
	void RandomBlocker( GameObject a_Object ) { };
	void HighWall( GameObject a_Object )
	{
		AddCollision( a_Object );
	};
	void LowWall( GameObject a_Object )
	{
		AddCollision( a_Object );
	};

private:
	void AddCollision( GameObject a_Object )
	{
		Vector3Int Coord = a_Object.GetTransform()->GetGlobalPosition();
		Level::GetActiveLevel()->GetAtlas().DisableNode( Coord.swizzle.xz );

	}

};

static LogicPopulator LP_;