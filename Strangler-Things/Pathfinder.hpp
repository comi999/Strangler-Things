#pragma once

#include <queue>
#include <list>
#include <map>

#include "Atlas.hpp"

class Pathfinder
{
private:

	typedef typename Atlas::Node Node;

	struct PFNode
	{
		uint32_t    GScore = 0;
		uint32_t    HScore = 0;
		const Node* This   = nullptr;
		PFNode*     Parent = nullptr;
	};

	struct NodeComparator
	{
		inline bool operator()( PFNode* A, PFNode* B )
		{
			return ( A->GScore + A->HScore ) > ( B->GScore + B->HScore );
		}
	};
	
	typedef std::priority_queue< 
		PFNode*, 
		std::vector< PFNode* >, 
		NodeComparator 
	> PriorityQueue;

public:

	Pathfinder( const Atlas& a_Atlas )
		: m_Atlas( &a_Atlas )
	{ }

	~Pathfinder()
	{
		Reset();
	}

	void AddGoal( Vector2UInt a_Coordinate )
	{
		m_Goals.push_back( a_Coordinate );
	}

	void ClearGoals()
	{
		m_Goals.clear();
	}

	uint32_t GetGoalCount()
	{
		return m_Goals.size();
	}

	bool StartSearch( Vector2UInt a_Start, int32_t a_MaxIterations = -1 )
	{
		// Reset
		Reset();

		if ( m_Goals.empty() )
		{
			return false;
		}

		m_MaxIterations = a_MaxIterations;
		m_InProgress = true;
		PFNode* NewPFNode = new PFNode();
		NewPFNode->This = m_Atlas->GetNode( a_Start );
		NewPFNode->HScore = CalculateHScore( a_Start );
		m_Queue.push( NewPFNode );
		m_Opened[ NewPFNode->This ] = NewPFNode;
	}

	void Reset()
	{
		m_CurrentIteration = 0;
		m_SearchComplete = false;
		m_FoundGoal = false;
		m_InProgress = false;
		for ( auto& PFNode : m_Opened ) delete PFNode.second;
		for ( auto& PFNode : m_Closed ) delete PFNode.second;
		m_Opened.clear();
		m_Closed.clear();
		m_Queue = PriorityQueue();
	}

	bool InProgress() const
	{
		return m_InProgress;
	}

	bool Tick( int32_t a_Steps = 1 )
	{
		for ( uint32_t i = 0; 
			  !m_Queue.empty() && 
			  ( a_Steps == -1 || i < a_Steps ) && 
			  ( m_MaxIterations == -1 || m_CurrentIteration < m_MaxIterations ); 
			  ++i, ++m_CurrentIteration )
		{
			PFNode* Top = m_Queue.top(); auto TopLocation = m_Atlas->GetNodeLocation( Top->This );

			// Check here if Top is the goal.
			if ( IsGoal( m_Atlas->GetNodeLocation( Top->This ) ) )
			{
				// Returning false means that the search ended prematurely.
				m_FoundGoal = true;
				m_SearchComplete = true;
				m_InProgress = false;
				return false;
			}

			// If not, remove from queue, remove from opened, and move into closed.
			m_Queue.pop();
			m_Opened.erase( m_Opened.find( Top->This ) );
			m_Closed[ Top->This ] = Top;

			// For each edge on top node, add into opened and queue only if it's not already closed.
			for ( uint32_t i = 0; i < 4; ++i )
			{
				Node* ThisEdge = Top->This->Edges[ i ];

				// If the edge is null or was already closed, continue.
				if ( !ThisEdge || m_Closed.find( ThisEdge ) != m_Closed.end() )
				{
					continue;
				}

				// Calculate this edges G score based on it's path.
				uint32_t GScore = Top->GScore + 1;

				// Check to see if edge is already in open list and swap paths if it has better scores.
				auto Found = m_Opened.find( ThisEdge );

				if ( Found != m_Opened.end() )
				{
					if ( GScore < Found->second->GScore )
					{
						Found->second->GScore = GScore;
						Found->second->Parent = Top;

						// Force priority queue resort
						Top = m_Queue.top();
						m_Queue.pop();
						m_Queue.push( Top );
					}

					continue;
				}

				// Create new PFNode for the edge and push into queue as well as opened.
				PFNode* NewPFNode = new PFNode();
				m_Opened[ ThisEdge ] = NewPFNode;
				NewPFNode->This   = ThisEdge;
				NewPFNode->GScore = GScore;
				NewPFNode->HScore = CalculateHScore( m_Atlas->GetNodeLocation( ThisEdge ) );;
				NewPFNode->Parent = Top;
				m_Queue.push( NewPFNode );
			}
		}

		// Returning true means that the search ran for the requested iterations.
		return true;
	}

	void AbandonSearch()
	{
		m_SearchComplete = true;
		m_InProgress = false;
	}

	bool FoundGoal() const
	{
		return m_FoundGoal;
	}

	bool SearchComplete() const
	{
		return m_SearchComplete;
	}

	bool GetPath( std::list< Vector2UInt >& a_PathRoute ) const
	{
		if ( m_Queue.empty() )
		{
			return false;
		}

		PFNode* Top = m_Queue.top();

		do
		{
			a_PathRoute.push_back( m_Atlas->GetNodeLocation( Top->This ) );
			Top = Top->Parent;
		} while ( Top );
	}

private:

	uint32_t CalculateHScore( Vector2UInt a_Coordinate ) const
	{
		uint32_t LowestHScore = -1;

		for ( auto Goal : m_Goals )
		{
			LowestHScore = Math::Min( LowestHScore, Math::DistanceSqrd( a_Coordinate, Goal ) );
		}

		return LowestHScore;
	}

	bool IsGoal( Vector2UInt a_Coordinate ) const
	{
		for ( auto Goal : m_Goals )
		{
			if ( Goal == a_Coordinate )
			{
				return true;
			}
		}

		return false;
	}

	friend class PathRoute;

	int32_t                          m_MaxIterations;
	int32_t                          m_CurrentIteration;
	bool                             m_SearchComplete;
	bool                             m_FoundGoal;
	bool                             m_InProgress;
	std::list< Vector2UInt >         m_Goals;
	std::map< const Node*, PFNode* > m_Opened;
	std::map< const Node*, PFNode* > m_Closed;
	std::vector< PFNode* >           m_PFNodes;
	const Atlas*                     m_Atlas;
	PriorityQueue                    m_Queue;
};