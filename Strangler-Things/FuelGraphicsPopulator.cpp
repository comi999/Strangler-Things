#include "FuelGraphicsPopulator.hpp"
#include "Resource.hpp"


void FuelGraphicsPopulator::Populate( GameObject& a_Object )
{
	static Material FuelMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "FuelMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::LIGHT_ORANGE );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}();

	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	GameObject NewFuel = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewFuel.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( FuelMaterial );

	Transform* NewTransform = NewFuel.GetTransform();
	NewTransform->SetParent( a_Object );
	NewTransform->SetLocalScale( Vector3::One * 0.66f );
}