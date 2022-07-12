#include "PlayerGraphicsPopulator.hpp"
#include "Resource.hpp"


void PlayerGraphicsPopulator::Populate( GameObject& a_Object )
{
	static Material PlayerMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "PlayerMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::GREEN );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}();

	// TODO: Why is this showing up white instead of green?
	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	GameObject NewPlayer = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewPlayer.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( PlayerMaterial );

	Transform* NewTransform = NewPlayer.GetTransform();
	NewTransform->SetParent( a_Object );
}