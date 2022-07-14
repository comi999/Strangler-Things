#pragma once

#include <vector>

#include "Math.hpp"

class Atlas
{
private:

	struct Node;
	typedef Node* Edge;

	struct Node
	{
		Edge Edges[ 4 ] { nullptr };
		bool Enabled = true;

		constexpr Edge& North() { return Edges[ 0 ]; }
		constexpr Edge& East () { return Edges[ 1 ]; }
		constexpr Edge& South() { return Edges[ 2 ]; }
		constexpr Edge& West () { return Edges[ 3 ]; }
	};

public:

	Atlas( Vector2UInt a_Size )
		: m_Size( a_Size )
	{
		m_Nodes.resize( ( size_t )m_Size.x * m_Size.y );

		// Generate Top and Bottom
		for ( uint32_t x = 1; x < m_Size.x - 1; ++x )
		{
			Node* TopNode    = GetNode( { x, m_Size.y - 1 } );
			Node* BottomNode = GetNode( { x, 0u           } );

			TopNode   ->East() = GetNode( { x + 1, m_Size.y - 1 } );
			TopNode   ->West() = GetNode( { x - 1, m_Size.y - 1 } );
			BottomNode->East() = GetNode( { x + 1, 0u           } );
			BottomNode->West() = GetNode( { x - 1, 0u           } );

			TopNode   ->South() = GetNode( { x, m_Size.y - 2 } );
			BottomNode->North() = GetNode( { x, 1u           } );
		}

		// Generate Left and Right
		for ( uint32_t y = 1; y < m_Size.y - 1; ++y )
		{
			Node* RightNode = GetNode( { m_Size.x - 1, y } );
			Node* LeftNode  = GetNode( { 0u,           y } );

			RightNode->North() = GetNode( { m_Size.x - 1, y + 1 } );
			RightNode->South() = GetNode( { m_Size.x - 1, y - 1 } );
			LeftNode ->North() = GetNode( { 0u          , y + 1 } );
			LeftNode ->South() = GetNode( { 0u          , y - 1 } );

			RightNode->West() = GetNode( { m_Size.x - 2, y } );
			LeftNode ->East() = GetNode( { 1u,           y } );
		}

		// Generate insides.
		for ( uint32_t y = 1; y < m_Size.y - 1; ++y )
		for ( uint32_t x = 1; x < m_Size.x - 1; ++x )
		{
			Node* ThisNode = GetNode( { x, y } );
			ThisNode->North() = GetNode( { x + 0, y + 1 } );
			ThisNode->East () = GetNode( { x + 1, y + 0 } );
			ThisNode->South() = GetNode( { x + 0, y - 1 } );
			ThisNode->West () = GetNode( { x - 1, y + 0 } );
		}

		// Generate corners
		Node* TLNode = GetNode( { 0u,           m_Size.y - 1 } );
		Node* TRNode = GetNode( { m_Size.x - 1, m_Size.y - 1 } );
		Node* BLNode = GetNode( { 0u,           0u } );
		Node* BRNode = GetNode( { m_Size.x - 1, 0u } );

		TLNode->East() = GetNode( { 1u, m_Size.y - 1 } );
		TLNode->South() = GetNode( { 0u, m_Size.y - 2 } );
		TRNode->West() = GetNode( { m_Size.x - 2, m_Size.y - 1 } );
		TRNode->South() = GetNode( { m_Size.x - 1, m_Size.y - 2 } );
		BLNode->North() = GetNode( { 0u, 1u } );
		BLNode->East() = GetNode( { 1u, 0u } );
		BRNode->North() = GetNode( { m_Size.x - 1, 1u } );
		BRNode->West() = GetNode( { m_Size.x - 2, 0u } );

	}

	inline bool CheckNode( Vector2UInt a_Coordinate ) const
	{
		return GetNode( a_Coordinate )->Enabled;
	}

	bool EnableNode( Vector2UInt a_Coordinate )
	{
		Node* ThisNode = GetNode( a_Coordinate );

		if ( ThisNode->Enabled )
		{
			return false;
		}

		// Establish link North
		if ( a_Coordinate.y < m_Size.y - 1 )
		{
			ThisNode->North() = GetNode( Vector2UInt{ a_Coordinate.x, a_Coordinate.y + 1 } );
			ThisNode->North()->South() = ThisNode;
		}

		// Establish link East
		if ( a_Coordinate.x < m_Size.x - 1 )
		{
			ThisNode->East() = GetNode( Vector2UInt{ a_Coordinate.x + 1, a_Coordinate.y } );
			ThisNode->East()->West() = ThisNode;
		}

		// Establish link South
		if ( a_Coordinate.y > 0 )
		{
			ThisNode->South() = GetNode( Vector2UInt{ a_Coordinate.x, a_Coordinate.y - 1 } );
			ThisNode->South()->North() = ThisNode;
		}

		// Establish link West
		if ( a_Coordinate.x > 0 )
		{
			ThisNode->West() = GetNode( Vector2UInt{ a_Coordinate.x - 1, a_Coordinate.y } );
			ThisNode->West()->East() = ThisNode;
		}
		
		ThisNode->Enabled = true;
		return true;
	}

	bool DisableNode( Vector2UInt a_Coordinate )
	{
		Node* ThisNode = GetNode( a_Coordinate );

		if ( !ThisNode->Enabled )
		{
			return false;
		}

		// Break link North
		if ( a_Coordinate.y < m_Size.y - 1 )
		{
			if ( ThisNode->North() ) ThisNode->North()->South() = nullptr;
			ThisNode->North() = nullptr;
		}

		// Break link East
		if ( a_Coordinate.x < m_Size.x - 1 )
		{
			if ( ThisNode->East() ) ThisNode->East()->West() = nullptr;
			ThisNode->East() = nullptr;
		}

		// Break link South
		if ( a_Coordinate.y > 0 )
		{
			if ( ThisNode->South() ) ThisNode->South()->North() = nullptr;
			ThisNode->South() = nullptr;
		}

		// Break link West
		if ( a_Coordinate.x > 0 )
		{
			if ( ThisNode->West() ) ThisNode->West()->East() = nullptr;
			ThisNode->West() = nullptr;
		}
		
		ThisNode->Enabled = false;
		return true;
	}

private:

	inline Node* GetNode( Vector2UInt a_Coordinate )
	{
		return &m_Nodes[ a_Coordinate.x + ( size_t )a_Coordinate.y * m_Size.x ];
	}

	inline const Node* GetNode( Vector2UInt a_Coordinate ) const
	{
		return &m_Nodes[ a_Coordinate.x + ( size_t )a_Coordinate.y * m_Size.x ];
	}

	Vector2UInt GetNodeLocation( const Node* a_Node ) const
	{
		uint32_t Offset = a_Node - &m_Nodes[ 0 ];
		Vector2UInt Location;
		Location.y = Offset / m_Size.x;
		Location.x = Offset - Location.y * m_Size.x;
		return Location;
	}

	friend class Pathfinder;

	std::vector< Node > m_Nodes;
	Vector2UInt         m_Size;
};