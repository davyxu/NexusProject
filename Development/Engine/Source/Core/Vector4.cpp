#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	//nVector4
	//////////////////////////////////////////////////////////////////////////
	const Vector4 Vector4::cZero( 0.0f, 0.0f, 0.0f, 0.0f );
	const Vector4 Vector4::cOne( 1.0f, 1.0f, 1.0f, 1.0f );
	const Vector4 Vector4::cXAxis( 1.0f, 0.0f, 0.0f, 0.0f );
	const Vector4 Vector4::cYAxis( 0.0f, 1.0f, 0.0f, 0.0f );
	const Vector4 Vector4::cZAxis( 0.0f, 0.0f, 1.0f, 0.0f );
	const Vector4 Vector4::cWAxis( 0.0f, 0.0f, 0.0f, 1.0f );
	const Vector4 Vector4::cMaxVector( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );
	const Vector4 Vector4::cMinVector( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );

	Vector4& Vector4::operator *= ( const Matrix4& matrix )
	{
		float xx = x, yy = y, zz = z, ww = w;

		x = xx * matrix( 0, 0 ) + yy * matrix( 1, 0 ) + zz * matrix( 2, 0 ) + ww * matrix( 3, 0 );
		y = xx * matrix( 0, 1 ) + yy * matrix( 1, 1 ) + zz * matrix( 2, 1 ) + ww * matrix( 3, 1 );
		z = xx * matrix( 0, 2 ) + yy * matrix( 1, 2 ) + zz * matrix( 2, 2 ) + ww * matrix( 3, 2 );
		w = xx * matrix( 0, 3 ) + yy * matrix( 1, 3 ) + zz * matrix( 2, 3 ) + ww * matrix( 3, 3 );

		return *this;
	}

	Vector4 Vector4::operator * ( const Matrix4& matrix ) const
	{
		Vector4 returnvector( *this );

		return returnvector *= matrix;
	}

	bool Vector4::operator == ( const Vector4& vector) const
	{
		const float margin = Math::cEpsilon ;

		if ( Math::Abs( x - vector.x ) >  margin )
			return false;

		if ( Math::Abs( y - vector.y ) >  margin )
			return false;

		if ( Math::Abs( z - vector.z ) >  margin )
			return false;

		if ( Math::Abs( w - vector.w ) >  margin )
			return false;

		return true;
	}

	Vector4& Vector4::Absolute( )
	{
		if ( x < 0.0f )
			x = -x;

		if ( y < 0.0f )
			y = -y;

		if ( z < 0.0f )
			z = -z;

		if ( w < 0.0f )
			w = -w;

		return *this;
	}

	Vector4& Vector4::Normalize( )
	{
		float mag = Magnitude( );

		if ( mag > 0.0f )
		{
			float magDiv = 1.0f / mag;
			x *= magDiv;
			y *= magDiv;
			z *= magDiv;
			w *= magDiv;
		}

		return *this;
	}

	float Vector4::Magnitude( ) const
	{
		return Math::Sqrt( x * x + y * y + z * z + w * w );
	}

	float Vector4::MagnitudeSquared( ) const
	{
		return x * x + y * y + z * z + w * w;
	}

	float Vector4::DotProduct( const Vector4& vector1, const Vector4& vector2 )
	{
		return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z + vector1.w * vector2.w;
	}

	Vector4 Vector4::Project( const Vector4& vector1, const Vector4& vector2 )
	{
		Vector4 normal = vector2;
		normal.Normalize( );

		return normal * DotProduct( vector1, normal );
	}

	Vector4 Vector4::Lerp( const Vector4& vector1, const Vector4& vector2, float factor )
	{
		factor = Math::Clamp( factor, 0.0f, 1.0f );

		return vector1 + ( vector2 - vector1 ) * factor;
	}



}