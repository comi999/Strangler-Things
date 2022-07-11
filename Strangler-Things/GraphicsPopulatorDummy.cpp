#include "GameObject.hpp"
#include "Prefab.hpp"
#include "Resource.hpp"

#include "GraphicsPopulatorDummy.hpp"


void GraphicsPopulatorRaw::LowWall( GameObject& Obj )
{
}

void GraphicsPopulatorRaw::HighWall( GameObject& Obj )
{
	auto Pre = Resource::Load< Prefab >( "cube"_H );
	GameObject Inst = Prefab::Instantiate( *Pre );
	Inst.GetTransform()->SetParent( Obj );
}