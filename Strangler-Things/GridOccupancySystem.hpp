#pragma once

#include <array>
#include <unordered_set>

#include "Math.hpp"


namespace std
{
	template< typename T, size_t N > 
	struct std::hash< Vector< T, N > >
	{
		auto operator() ( const Vector< T, N >& key ) const
		{
			// Thanks: https://codereview.stackexchange.com/questions/171999/specializing-stdhash-for-stdarray

			std::hash< T > hasher;
			size_t result = 0;
			
			for( size_t i = 0; i < key.GetSize(); ++i ) {
				result = result * 31 + hasher( key[i] );
			}

			return result;
		}
	};
}


class GridOccupancySystem
{
public:
	void RegisterOccupied( Vector3Int a_Coord )
	{
		_STL_ASSERT( m_OccupiedCoords.find( a_Coord ) == m_OccupiedCoords.end(), "Double-registering occupied coord!" );
		m_OccupiedCoords.insert( a_Coord );
	}

	void DeregisterOccupied( Vector3Int a_Coord )
	{
		_STL_ASSERT( m_OccupiedCoords.find( a_Coord ) != m_OccupiedCoords.end(), "Deregistering non-occupied!" );
		m_OccupiedCoords.erase( a_Coord );
	}

	bool CoordIsOccupied( Vector3Int a_Coord )
	{
		return m_OccupiedCoords.find( a_Coord ) != m_OccupiedCoords.end();
	}

	bool CoordIsOccupied( Vector3 a_Position )
	{
		Vector3Int RoundedCoord = Vector3(
			Math::Round( a_Position.x ),
			Math::Round( a_Position.y ),
			Math::Round( a_Position.z )
		);
		return CoordIsOccupied( RoundedCoord );
	}

private:
	std::unordered_set< Vector3Int > m_OccupiedCoords;

};