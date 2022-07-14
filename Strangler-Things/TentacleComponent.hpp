#pragma once

#include <map>
#include <vector>

#include "Component.hpp"
#include "GameObject.hpp"
#include "Math.hpp"

#include "GraphicsPopulator.hpp"
#include "Level.hpp"


static Vector2Int s_TentacleGrowthDirections[]
{
	Vector2Int::Up,
	Vector2Int::Right,
	Vector2Int::Down,
	Vector2Int::Left
};

class TentacleNode
{
public:
	TentacleNode( GameObject a_RootObject, Vector3Int a_Coord, Vector2Int a_PreviousDirection )
		: Coord( a_Coord ), PreviousDirection( a_PreviousDirection ), Children()
	{
		Level::GetActiveLevel()->GetAtlas().DisableNode( Coord.swizzle.xz );

		Obj = GameObject::Instantiate( a_RootObject );
		Obj.GetTransform()->SetGlobalPosition( Coord );
		GP.TentacleNode_( Obj, this );

	}

	GameObject Obj;
	Vector3Int Coord;
	Vector2Int PreviousDirection;
	std::vector< TentacleNode > Children;

};

DefineComponent( TentacleComponent, Component )
{
public:
	void Init()
	{
		GameObject* Obj = GetOwner();
		Vector3Int Coord = Obj->GetTransform()->GetGlobalPosition();

		for ( Vector2Int Dir : s_TentacleGrowthDirections )
		{
			Vector3Int Temp = (Vector3Int)Dir;
			// TODO: LEN!!!
			// TentacleNode NewNode( *Obj, Coord + ( (Vector3Int)Dir ).swizzle.xzy.ToVector(), Dir * -1 );
			Vector3Int NewCoord = Coord + Vector3Int( Dir.x, 0, Dir.y );
			TentacleNode NewNode( *Obj, NewCoord, Dir * -1 );
			m_AllNodes.emplace( std::make_pair( NewCoord, std::move( NewNode ) ) );
			m_LeafNodes.push_back( NewCoord );
		}

	}

private:
	struct Vector3IntCompare
	{
		bool operator()( const Vector3Int& a, const Vector3Int& b ) const
		{
			if (a.x == b.x)
			{
				if (a.y == b.y)
				{
					return a.z < b.z;
				}

				return a.y < b.y;
			}

			return a.x < b.x;
		}
	};

	std::map< Vector3Int, TentacleNode, Vector3IntCompare > m_AllNodes;
	std::vector< Vector3Int > m_LeafNodes;

};