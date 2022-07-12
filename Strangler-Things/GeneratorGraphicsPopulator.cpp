#include "GeneratorGraphicsPopulator.hpp"
#include "Resource.hpp"


void GeneratorGraphicsPopulator::Populate( GameObject& a_Object )
{
	static Material GeneratorMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "GeneratorMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::LIGHT_BLUE );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}();

	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	GameObject NewGenerator = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewGenerator.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( GeneratorMaterial );

	Transform* NewTransform = NewGenerator.GetTransform();
	NewTransform->SetParent( a_Object );
	NewTransform->SetLocalScale( Vector3( 0.33f, 3.0f, 0.33f ) );
}