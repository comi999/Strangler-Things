#include "Light.hpp"

#include "GraphicsPopulator.hpp"
#include "TentacleComponent.hpp"


void GraphicsPopulator::Scene( GameObject a_Object, Hash a_LevelName )
{
	// Create Sun
	GameObject SunObject = GameObject::Instantiate( "Sun"_N, a_Object );
	Light* SunComponent = SunObject.AddComponent< Light >();
	SunComponent->SetDirection( Math::Normalize( Vector3::Down + Vector3::Right + Vector3::Forward * 0.3f ) );
	Light::SetSun( SunComponent );
};
void GraphicsPopulator::Abyss( GameObject a_Object ) { GraphicsPopulatorBase::Abyss( a_Object ); };
void GraphicsPopulator::Floor( GameObject a_Object ) { GraphicsPopulatorBase::Floor( a_Object ); };
void GraphicsPopulator::HighWall( GameObject a_Object ) { GraphicsPopulatorBase::HighWall( a_Object ); };
void GraphicsPopulator::LowWall( GameObject a_Object ) { GraphicsPopulatorBase::LowWall( a_Object ); };
void GraphicsPopulator::HorizontalExit( GameObject a_Object ) { GraphicsPopulatorBase::HorizontalExit( a_Object ); };
void GraphicsPopulator::Player( GameObject a_Object ) { GraphicsPopulatorBase::Player( a_Object ); };
void GraphicsPopulator::TentacleStart( GameObject a_Object ) { GraphicsPopulatorBase::TentacleStart( a_Object ); };
void GraphicsPopulator::TentacleNode_( GameObject a_Object, TentacleNode* a_TentacleNode ) { GraphicsPopulatorBase::TentacleNode_( a_Object, a_TentacleNode ); };
void GraphicsPopulator::Random( GameObject a_Object ) { GraphicsPopulatorBase::Random( a_Object ); };
void GraphicsPopulator::Fuel( GameObject a_Object ) { GraphicsPopulatorBase::Fuel( a_Object ); };
void GraphicsPopulator::Generator( GameObject a_Object ) { GraphicsPopulatorBase::Generator( a_Object ); };
void GraphicsPopulator::Bonus( GameObject a_Object ) { GraphicsPopulatorBase::Bonus( a_Object ); };
void GraphicsPopulator::RandomBlocker( GameObject a_Object ) { GraphicsPopulatorBase::RandomBlocker( a_Object ); };