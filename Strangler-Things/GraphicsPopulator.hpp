#pragma once

#include "GraphicsPopulatorBase.hpp"


class GraphicsPopulator : public GraphicsPopulatorBase
{
public:
	void Scene( GameObject a_Object, Hash a_LevelName ) override
	{
		// Create Sun
		GameObject SunObject = GameObject::Instantiate( "Sun"_N, a_Object );
		Light* SunComponent = SunObject.AddComponent< Light >();
		SunComponent->SetDirection( Math::Normalize( Vector3::Down + Vector3::Right + Vector3::Forward * 0.3f ) );
		Light::SetSun( SunComponent );
	};
	void Abyss( GameObject a_Object ) override { GraphicsPopulatorBase::Abyss( a_Object ); };
	void Floor( GameObject a_Object ) override { GraphicsPopulatorBase::Floor( a_Object ); };
	void HighWall( GameObject a_Object ) override { GraphicsPopulatorBase::HighWall( a_Object ); };
	void LowWall( GameObject a_Object ) override { GraphicsPopulatorBase::LowWall( a_Object ); };
	void HorizontalExit( GameObject a_Object ) override { GraphicsPopulatorBase::HorizontalExit( a_Object ); };
	void Player( GameObject a_Object ) override { GraphicsPopulatorBase::Player( a_Object ); };
	void TentacleStart( GameObject a_Object ) override { GraphicsPopulatorBase::TentacleStart( a_Object ); };
	void Random( GameObject a_Object ) override { GraphicsPopulatorBase::Random( a_Object ); };
	void Fuel( GameObject a_Object ) override { GraphicsPopulatorBase::Fuel( a_Object ); };
	void Generator( GameObject a_Object ) override { GraphicsPopulatorBase::Generator( a_Object ); };
	void Bonus( GameObject a_Object ) override { GraphicsPopulatorBase::Bonus( a_Object ); };
	void RandomBlocker( GameObject a_Object ) override { GraphicsPopulatorBase::RandomBlocker( a_Object ); };

};

static GraphicsPopulator GP;