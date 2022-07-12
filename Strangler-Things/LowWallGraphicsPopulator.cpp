#include "LowWallGraphicsPopulator.hpp"
#include "Resource.hpp"


void LowWallGraphicsPopulator::Populate( GameObject& a_Object )
{
	static Material LowWallMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "LowWallMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::WHITE );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}();

	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	GameObject NewLowWall = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewLowWall.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( LowWallMaterial );

	Vector4 c;
	bool success = NewRenderer->GetMaterial()->GetProperty( "diffuse_colour"_H, c );

	Transform* NewTransform = NewLowWall.GetTransform();
	NewTransform->SetParent( a_Object );
}