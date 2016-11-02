#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	//Vector2
	//////////////////////////////////////////////////////////////////////////

	const Vector2 Vector2::cZero( 0.0f, 0.0f );
	const Vector2 Vector2::cOne( 1.0f, 1.0f );
	const Vector2 Vector2::cXAxis( 1.0f, 0.0f );
	const Vector2 Vector2::cYAxis( 0.0f, 1.0f );

	bool Vector2::Equal( const Vector2& vector, float margin ) const
	{
		if ( Math::Abs( x - vector.x ) > margin )
			return false;

		if ( Math::Abs( y - vector.y ) > margin )
			return false;

		return true;
	}

	Vector2& Vector2::Normalize( )
	{
		float mag = Magnitude( );

		if ( mag > 0.0f )
		{
			x /= mag;
			y /= mag;
		}

		return *this;
	}

	float Vector2::Magnitude( ) const
	{
		return Math::Sqrt( x * x + y * y );
	}

	float Vector2::MagnitudeSquared( ) const
	{
		return x * x + y * y;
	}

	float Vector2::DotProduct( const Vector2& vector1, const Vector2& vector2 )
	{
		return vector1.x * vector2.x + vector1.y * vector2.y;
	}

	float Vector2::CrossProduct( const Vector2& vector1, const Vector2& vector2 )
	{
		return vector1.x * vector2.y - vector1.y * vector2.x;
	}

	Vector2 Vector2::Project( const Vector2& vector1, const Vector2& vector2 )
	{
		Vector2 normal = vector2;
		normal.Normalize( );

		return normal * DotProduct( vector1, normal );
	}

	Vector2 Vector2::Lerp( const Vector2& vector1, const Vector2& vector2, float factor )
	{
		factor = Math::Clamp( factor, 0.0f, 1.0f );

		return vector1 + ( vector2 - vector1 ) * factor;
	}

	float Vector2::DistanceTo(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		return Math::Sqrt(dx * dx + dy * dy );
	}


	float Vector2::DistanceToSqare(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		return dx * dx + dy * dy ;
	}

	Vector2& Vector2::operator *= ( const Matrix3& matrix )
	{
		float xx = x, yy = y;

		x = xx * matrix.m11 + yy * matrix.m21;
		y = xx * matrix.m12 + yy * matrix.m22;

		return *this;
	}

	Vector2& Vector2::Mul2D( const Matrix3& matrix )
	{
		float xx = x, yy = y;

		float w = xx * matrix.m13 + yy * matrix.m23 + matrix.m33;

		x = xx * matrix.m11 + yy * matrix.m21 + matrix.m31;
		y = xx * matrix.m12 + yy * matrix.m22 + matrix.m32;

		x /= w;
		y /= w;

		return *this;
	}

	Vector2 Vector2::operator * ( const Matrix3& matrix ) const
	{
		Vector2 returnvector( *this );

		return returnvector *= matrix;
	}

	bool Vector2::operator == (const Vector2 &v) const
	{
		return x == v.x && y == v.y;
	}

	bool Vector2::operator != (const Vector2 &v) const
	{
		return x != v.x || y != v.y ;
	}

}