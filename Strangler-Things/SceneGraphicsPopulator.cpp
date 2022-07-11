#include "SceneGraphicsPopulator.hpp"
#include "Resource.hpp"

void SceneGraphicsPopulator::Populate( GameObject& a_Object )
{
	// Create Sun
	GameObject SunObject = GameObject::Instantiate( "Sun"_N, a_Object );
	Light* SunComponent = SunObject.AddComponent< Light >();
	SunComponent->SetDirection( Math::Normalize( Vector3::Down + Vector3::Right + Vector3::Forward * 0.3f ) );
	Light::SetSun( SunComponent );
}