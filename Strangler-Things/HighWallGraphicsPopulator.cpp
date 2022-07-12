#include "HighWallGraphicsPopulator.hpp"


void HighWallGraphicsPopulator::Populate( GameObject& a_Object )
{
	static ResourceHandle< Material > HighWallMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "HighWallMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::WHITE );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}( );

	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	
	for ( int i = 0; i < 2; ++i )
	{
		GameObject NewHighWall = Prefab::Instantiate( *Cube );
		MeshRenderer* NewRenderer = NewHighWall.GetComponentInChild< MeshRenderer >();
		NewRenderer->SetMaterial( HighWallMaterial );
		Transform* NewTransform = NewHighWall.GetTransform();
		NewTransform->SetGlobalPositionY( i );
		NewTransform->SetParent( a_Object );
	}
}