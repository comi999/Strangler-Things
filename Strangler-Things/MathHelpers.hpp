#pragma once

#include "Math.hpp"


template< typename T, size_t S >
struct std::hash< Vector< T, S > >
{
	auto operator()( const Vector< T, S >& Vec ) const
	{
		// Thanks: https://stackoverflow.com/a/27216842
		size_t Seed = Vec.GetSize();
		for ( size_t I = 0; I < Vec.GetSize(); ++I )
		{
			Seed ^= Vec[ I ] + 0x9e3779b9 + ( Seed << 6 ) + ( Seed >> 2 );
		}
		return Seed;
	}
};

struct MathHelpers
{
	static Vector3UInt PositionToCoord3( Vector3 a_Position )
	{
		return Vector3UInt(
			Math::Round( a_Position.x ),
			Math::Round( a_Position.y ),
			Math::Round( a_Position.z )
		);
	}

	static Vector2UInt PositionToCoord2( Vector3 a_Position )
	{
		return Vector2UInt(
			Math::Round( a_Position.x ),
			Math::Round( a_Position.z )
		);
	}
};