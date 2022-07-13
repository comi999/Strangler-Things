#include <functional>
#include <memory>

#include "HandleExitStatusChangedComponent.hpp"
#include "HorizontalExitGraphicsPopulator.hpp"
#include "Resource.hpp"


GameObject CreateExitCube( GameObject& a_Parent )
{
	static ResourceHandle< Material > ExitMaterial = []()
	{
		Material Temp = Material::LitFlatColour;
		Temp.SetName( "ExitMaterial"_N );
		Temp.SetProperty( "diffuse_colour"_H, ( Vector4 )Colour::BLUE );
		Temp.SetShader( Shader::LitFlatColour );
		return Temp;
	}();

	Prefab* Cube = Resource::Load< Prefab >( "cube"_H ).Assure();
	GameObject NewCube = Prefab::Instantiate( *Cube );
	MeshRenderer* NewRenderer = NewCube.GetComponentInChild< MeshRenderer >();
	NewRenderer->SetMaterial( ExitMaterial );
	Transform* NewTransform = NewCube.GetTransform();
	NewTransform->SetParent( a_Parent, false );

	return NewCube;
}

void HorizontalExitGraphicsPopulator::Populate( GameObject& a_Object )
{
	Vector3 DoorCoord = a_Object.GetTransform()->GetLocalPosition();

	GameObject LeftCubeObj = CreateExitCube( a_Object );
	Transform* LeftCube = LeftCubeObj.GetTransform();
	LeftCube->SetLocalPositionX( -1.0f );
	LeftCube->SetLocalScale( Vector3( 1.0f, 2.2f, 1.2f ) );
	
	GameObject RightCubeObj = CreateExitCube( a_Object );
	Transform* RightCube = RightCubeObj.GetTransform();
	RightCube->SetLocalPositionX( 1.0f );
	RightCube->SetLocalScale( Vector3( 1.0f, 2.2f, 1.2f ) );
	
	GameObject TopCubeObj = CreateExitCube( a_Object );
	Transform* TopCube = TopCubeObj.GetTransform();
	TopCube->SetLocalPositionY( 1.5f );
	TopCube->SetLocalScale( Vector3( 1.0f, 0.6f, 1.2f ) );

	GameplaySystem::OnExitStatusChanged += [=]()
	{
		if (GameplaySystem::IsExitOpen())
		{
			GameplaySystem::GetMatch().lock()->GetGridOccupancySystem().DeregisterOccupied( DoorCoord );
		}
		else
		{
			GameplaySystem::GetMatch().lock()->GetGridOccupancySystem().RegisterOccupied( DoorCoord );
		}
	};
}