#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "Component.hpp"
#include "GameObject.hpp"
#include "Math.hpp"

#include "GraphicsPopulator.hpp"
#include "Level.hpp"
#include "MathHelpers.hpp"
#include "PlayerComponent.hpp"


class TentacleSystem;

static std::array< Vector2Int, 4 > s_TentacleGrowthDirections
{
	Vector2Int::Up,
	Vector2Int::Right,
	Vector2Int::Down,
	Vector2Int::Left
};

static std::array< Vector2Int, 4 > s_RandomGrowthDirections = s_TentacleGrowthDirections;

class TentacleNode
{
public:
	TentacleNode( GameObject a_RootObject, Vector3Int a_Coord, Vector2Int a_PreviousDirection )
		: Coord( a_Coord ), PreviousDirection( a_PreviousDirection ), Children()
	{
		Level::GetActiveLevel()->GetAtlas().DisableNode( Coord.swizzle.xz );
		
		m_RootObj = a_RootObject;
		Obj = GameObject::Instantiate( m_RootObj );
		Obj.GetTransform()->SetGlobalPosition( Coord );
		GP.TentacleNode_( Obj, this );

	}

	bool HasRoomToGrow()
	{
		auto& Atlas = Level::GetActiveLevel()->GetAtlas();

		if ( IsRandomTarget() )
		{
			for ( Vector2Int Dir : s_TentacleGrowthDirections )
			{
				Vector3UInt NewCoord = Coord + Vector3Int( Dir.x, 0, Dir.y );
				if ( CanGrowOnCoord( NewCoord ) )
				{
					return true;
				}
			}

			return false;
		}
		else
		{
			// TODO: Implement with pathing support!
			return false;
		}
	}

	TentacleNode Grow()
	{
		if ( IsRandomTarget() )
		{
			Vector2Int Dir = GetRandomUnblockedDirection();
			Vector3Int NewCoord = Coord + Vector3Int( Dir.x, 0, Dir.y );
			return TentacleNode( m_RootObj, NewCoord, Dir * -1 );
		}
		else
		{
			// TODO: Implement with pathing support!
			throw std::exception();
		}
	}

	const bool IsRandomTarget() const
	{
		return !Target.IsValid();
	}

	GameObject Obj;
	Vector3Int Coord;
	Vector2Int PreviousDirection;
	std::vector< TentacleNode > Children;
	GameObject Target;

private:
	GameObject m_RootObj;

	Vector2Int GetRandomUnblockedDirection()
	{
		// Adapted from: https://cplusplus.com/reference/algorithm/shuffle/

		unsigned Seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle ( s_RandomGrowthDirections.begin(), s_RandomGrowthDirections.end(), std::default_random_engine( Seed ) );
		

		for ( Vector2Int Dir : s_RandomGrowthDirections )
		{
			Vector3UInt NewCoord = Coord + Vector3Int( Dir.x, 0, Dir.y );
			if ( CanGrowOnCoord( NewCoord ) )
			{
				return Dir;
			}
		}

		throw std::exception();
	}

	bool CanGrowOnCoord( Vector3UInt Coord )
	{
		// TODO: LEN!!!
		// if ( Atlas.CheckNode( NewCoord.swizzle.xz.ToVector() ) )
		if ( ! Level::GetActiveLevel()->GetAtlas().CheckNode( Vector2UInt( Coord.x, Coord.z ) ) )
		{
			return false;
		}

		for ( auto& Player : Component::GetComponents< PlayerComponent >() )
		{
			if ( MathHelpers::PositionToCoord3( Player->GetTransform()->GetGlobalPosition() ) == Coord )
			{
				return false;
			}
		}

		return true;
	}

};

DefineComponent( TentacleComponent, Component )
{
	friend TentacleSystem;

public:
	void Init()
	{
		m_NextGrowthAfterSeconds = GROWTH_INTERVAL_SECONDS;
		CreateInitialNodes();
		SetInitialNodeTargets();

	}

	void Grow()
	{
		m_NextGrowthAfterSeconds = GROWTH_INTERVAL_SECONDS;
		
		std::unordered_set< Vector3Int > OldLeaves;
		std::unordered_set< Vector3Int > NewLeaves;

		for ( Vector3Int LeafNodeCoord : m_LeafNodes )
		{
			auto& Leaf = ( *m_AllNodes.find( LeafNodeCoord ) ).second;

			if ( Leaf.HasRoomToGrow() )
			{
				OldLeaves.insert( LeafNodeCoord );

				TentacleNode NewLeaf = Leaf.Grow();
				NewLeaves.insert( NewLeaf.Coord );
				m_AllNodes.emplace( std::make_pair( NewLeaf.Coord, std::move( NewLeaf ) ) );
			}
		}

		for ( Vector3Int OldLeaf : OldLeaves )
		{
			m_LeafNodes.erase( OldLeaf );
		}

		for ( Vector3Int NewLeaf : NewLeaves )
		{
			m_LeafNodes.insert( NewLeaf );
		}

	}

private:
	static const int GROWTH_INTERVAL_SECONDS = 4.0f;

	float m_NextGrowthAfterSeconds;
	std::unordered_map< Vector3Int, TentacleNode > m_AllNodes;
	std::unordered_set< Vector3Int > m_LeafNodes;

	void CreateInitialNodes()
	{
		GameObject* Obj = GetOwner();
		Vector3Int Coord = Obj->GetTransform()->GetGlobalPosition();

		for ( Vector2Int Dir : s_TentacleGrowthDirections )
		{
			// TODO: LEN!!!
			// TentacleNode NewNode( *Obj, Coord + ( (Vector3Int)Dir ).swizzle.xzy.ToVector(), Dir * -1 );
			Vector3Int NewCoord = Coord + Vector3Int( Dir.x, 0, Dir.y );
			TentacleNode NewNode( *Obj, NewCoord, Dir * -1 );
			m_AllNodes.emplace( std::make_pair( NewCoord, std::move( NewNode ) ) );
			m_LeafNodes.insert( NewCoord );
		}

	}

	void SetInitialNodeTargets()
	{
		// TODO!!

	}

};