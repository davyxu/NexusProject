#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	const Matrix3 Matrix3::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	const Matrix3 Matrix3::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );

	Matrix3& Matrix3::operator += ( const Matrix3& matrix )
	{
		m[0][0] += matrix.m[0][0]; m[0][1] += matrix.m[0][1]; m[0][2] += matrix.m[0][2];
		m[1][0] += matrix.m[1][0]; m[1][1] += matrix.m[1][1]; m[1][2] += matrix.m[1][2];
		m[2][0] += matrix.m[2][0]; m[2][1] += matrix.m[2][1]; m[2][2] += matrix.m[2][2];

		return *this;
	}

	Matrix3& Matrix3::operator -= ( const Matrix3& matrix )
	{
		m[0][0] -= matrix.m[0][0]; m[0][1] -= matrix.m[0][1]; m[0][2] -= matrix.m[0][2];
		m[1][0] -= matrix.m[1][0]; m[1][1] -= matrix.m[1][1]; m[1][2] -= matrix.m[1][2];
		m[2][0] -= matrix.m[2][0]; m[2][1] -= matrix.m[2][1]; m[2][2] -= matrix.m[2][2];

		return *this;
	}

	Matrix3& Matrix3::operator *= ( const Matrix3& matrix )
	{
		float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m10 = m[1][0], m11 = m[1][1],
			m12 = m[1][2], m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

		m[0][0] = m00 * matrix.m[0][0] + m01 * matrix.m[1][0] + m02 * matrix.m[2][0];
		m[0][1] = m00 * matrix.m[0][1] + m01 * matrix.m[1][1] + m02 * matrix.m[2][1];
		m[0][2] = m00 * matrix.m[0][2] + m01 * matrix.m[1][2] + m02 * matrix.m[2][2];

		m[1][0] = m10 * matrix.m[0][0] + m11 * matrix.m[1][0] + m12 * matrix.m[2][0];
		m[1][1] = m10 * matrix.m[0][1] + m11 * matrix.m[1][1] + m12 * matrix.m[2][1];
		m[1][2] = m10 * matrix.m[0][2] + m11 * matrix.m[1][2] + m12 * matrix.m[2][2];

		m[2][0] = m20 * matrix.m[0][0] + m21 * matrix.m[1][0] + m22 * matrix.m[2][0];
		m[2][1] = m20 * matrix.m[0][1] + m21 * matrix.m[1][1] + m22 * matrix.m[2][1];
		m[2][2] = m20 * matrix.m[0][2] + m21 * matrix.m[1][2] + m22 * matrix.m[2][2];

		return *this;
	}

	Matrix3& Matrix3::operator *= ( float scalar )
	{
		m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar;
		m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar;
		m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar;

		return *this;
	}

	Matrix3& Matrix3::operator /= ( float scalar )
	{
		m[0][0] /= scalar; m[0][1] /= scalar; m[0][2] /= scalar;
		m[1][0] /= scalar; m[1][1] /= scalar; m[1][2] /= scalar;
		m[2][0] /= scalar; m[2][1] /= scalar; m[2][2] /= scalar;

		return *this;
	}

	Matrix3 Matrix3::operator + ( const Matrix3& matrix ) const
	{
		Matrix3 returnmatrix = *this;

		return returnmatrix += matrix;
	}

	Matrix3 Matrix3::operator - ( const Matrix3& matrix ) const
	{
		Matrix3 returnmatrix = *this;

		return returnmatrix -= matrix;
	}

	Matrix3 Matrix3::operator * ( const Matrix3& matrix ) const
	{
		Matrix3 returnmatrix = *this;

		return returnmatrix *= matrix;
	}

	Matrix3 Matrix3::operator * ( float scalar ) const
	{
		Matrix3 returnmatrix = *this;

		return returnmatrix *= scalar;
	}

	Matrix3 Matrix3::operator / ( float scalar ) const
	{
		Matrix3 returnmatrix = *this;

		return returnmatrix /= scalar;
	}

	Matrix3 Matrix3::Identity( )
	{
		return Matrix3::cIdentity;
	}

	Matrix3 Matrix3::Transpose( )
	{
		return Matrix3(m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]);
	} 

	Matrix3 Matrix3::Inverse( )
	{
		// Invert a 3x3 using cofactors.  This is about 8 times faster than
		// the Numerical Recipes code which uses Gaussian elimination.

		Matrix3 rkInverse;

		rkInverse.m[0][0] = m[1][1]*m[2][2] -
			m[1][2]*m[2][1];
		rkInverse.m[0][1] = m[0][2]*m[2][1] -
			m[0][1]*m[2][2];
		rkInverse.m[0][2] = m[0][1]*m[1][2] -
			m[0][2]*m[1][1];
		rkInverse.m[1][0] = m[1][2]*m[2][0] -
			m[1][0]*m[2][2];
		rkInverse.m[1][1] = m[0][0]*m[2][2] -
			m[0][2]*m[2][0];
		rkInverse.m[1][2] = m[0][2]*m[1][0] -
			m[0][0]*m[1][2];
		rkInverse.m[2][0] = m[1][0]*m[2][1] -
			m[1][1]*m[2][0];
		rkInverse.m[2][1] = m[0][1]*m[2][0] -
			m[0][0]*m[2][1];
		rkInverse.m[2][2] = m[0][0]*m[1][1] -
			m[0][1]*m[1][0];

		float fDet =
			m[0][0]*rkInverse.m[0][0] +
			m[0][1]*rkInverse.m[1][0]+
			m[0][2]*rkInverse.m[2][0];

		if ( Math::Abs(fDet) <= Math::cEpsilon )
			return rkInverse;

		float fInvDet = 1.0f/fDet;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				rkInverse.m[iRow][iCol] *= fInvDet;
		}

		return rkInverse;
	}

	Matrix3& Matrix3::Scaling( float x, float y, float z )
	{
		m[0][0]	=    x; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] =    y; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] =    z;

		return *this;
	}

	Matrix3& Matrix3::Scaling2D( float x, float y )
	{
		m[0][0]	=    x; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] =    y; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

		return *this;
	}

	Matrix3& Matrix3::Scaling2D( const Vector2& vector )
	{
		return Scaling2D( vector.x, vector.y );
	}

	Matrix3& Matrix3::Scaling( const Vector3& vector )
	{
		return Scaling( vector.x, vector.y, vector.z );
	}

	Matrix3& Matrix3::Scaling( const Vector3& vector, float factor )
	{
		Vector3 normal( vector );
		normal.Normalize( );

		float k  = factor - 1.0f;
		float xx = normal.x * normal.x;
		float yy = normal.y * normal.y;
		float zz = normal.z * normal.z;
		float xy = normal.x * normal.y;
		float yz = normal.y * normal.z;
		float zx = normal.z * normal.x;

		m[0][0]	= 1.0f + k * xx; m[0][1] =        k * xy; m[0][2] =        k * zx;
		m[1][0]	=        k * xy; m[1][1] = 1.0f + k * yy; m[1][2] =        k * yz;
		m[2][0]	=        k * zx; m[2][1] =        k * yz; m[2][2] = 1.0f + k * zz;

		return *this;
	}

	Vector3 Matrix3::operator* (const Vector3& v) const
	{
		Vector3 final;

		final.x = m11 * v.x + m12 * v.y + m13 * v.z;
		final.y = m21 * v.x + m22 * v.y + m23 * v.z;
		final.z = m31 * v.x + m32 * v.y + m33 * v.z;

		return final;
	}

	Matrix3& Matrix3::RotationX( float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

		m[0][0]	= 1.0f; m[0][1] =       0.0f; m[0][2] =     0.0f;
		m[1][0] = 0.0f; m[1][1] =   cosvalue; m[1][2] = sinvalue;
		m[2][0] = 0.0f; m[2][1] = - sinvalue; m[2][2] = cosvalue;

		return *this;
	}

	Matrix3& Matrix3::RotationY( float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

		m[0][0]	= cosvalue; m[0][1] = 0.0f; m[0][2] = - sinvalue;
		m[1][0] =     0.0f; m[1][1] = 1.0f; m[1][2] =       0.0f;
		m[2][0] = sinvalue; m[2][1] = 0.0f; m[2][2] =   cosvalue;

		return *this;
	}

	Matrix3& Matrix3::RotationZ( float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

		m[0][0]	=   cosvalue; m[0][1] = sinvalue; m[0][2] = 0.0f;
		m[1][0] = - sinvalue; m[1][1] = cosvalue; m[1][2] = 0.0f;
		m[2][0] =       0.0f; m[2][1] =     0.0f; m[2][2] = 1.0f;

		return *this;
	}

	Matrix3& Matrix3::RotationAxis( const Vector3& axis, float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian ), cosreverse = 1.0f - cosvalue;

		Vector3 axisnormal( axis );
		axisnormal.Normalize( );

		m[0][0] = cosreverse * axisnormal.x * axisnormal.x + cosvalue;
		m[0][1] = cosreverse * axisnormal.x * axisnormal.y + sinvalue * axisnormal.z;
		m[0][2] = cosreverse * axisnormal.x * axisnormal.z - sinvalue * axisnormal.y;

		m[1][0] = cosreverse * axisnormal.x * axisnormal.y - sinvalue * axisnormal.z;
		m[1][1] = cosreverse * axisnormal.y * axisnormal.y + cosvalue;
		m[1][2] = cosreverse * axisnormal.y * axisnormal.z + sinvalue * axisnormal.x;

		m[2][0] = cosreverse * axisnormal.x * axisnormal.z + sinvalue * axisnormal.y;
		m[2][1] = cosreverse * axisnormal.y * axisnormal.z - sinvalue * axisnormal.x;
		m[2][2] = cosreverse * axisnormal.z * axisnormal.z + cosvalue;

		return *this;
	}

	Matrix3& Matrix3::Translation2D( float x, float y)
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f;  m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f;  m[1][2] = 0.0f;
		m[2][0] =    x; m[2][1] =    y;  m[2][2] = 1.0f;
		

		return *this;
	}

	Matrix3& Matrix3::SetTranslation2D(  float x, float y )
	{
		m[2][0] =    x; m[2][1] =    y;  m[2][2] = 1.0f;
		return *this;
	}

	Matrix3& Matrix3::SetTranslation2D( const Vector2& vector )
	{
		m[2][0] =    vector.x; m[2][1] =    vector.y;  m[2][2] = 1.0f;
		return *this;
	}

	Matrix3& Matrix3::AddTranslation2D( const Vector2& vector )
	{
		m[2][0] +=    vector.x; m[2][1] +=    vector.y;  m[2][2] = 1.0f;
		return *this;
	}

	Matrix3& Matrix3::Translation2D( const Vector2& vector )
	{
		return Translation2D( vector.x, vector.y  );
	}

	Matrix3& Matrix3::ProjectXY( )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f;

		return *this;
	}

	Matrix3& Matrix3::ProjectYZ( )
	{
		m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

		return *this;
	}

	Matrix3& Matrix3::ProjectZX( )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

		return *this;
	}

	Matrix3& Matrix3::Project( const Vector3& vector )
	{
		return Scaling( vector, 0.0f );
	}

	Matrix3& Matrix3::RefectionXY( )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] =  0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] =  0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = -1.0f;

		return *this;
	}

	Matrix3& Matrix3::RefectionYZ( )
	{
		m[0][0] = -1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] =  0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
		m[2][0] =  0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

		return *this;
	}

	Matrix3& Matrix3::RefectionZX( )
	{
		m[0][0] = 1.0f; m[0][1] =  0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = -1.0f; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] =  0.0f; m[2][2] = 1.0f;

		return *this;
	}

	Matrix3& Matrix3::Refection( const Vector3& vector )
	{
		return Scaling( vector, -1.0f );
	}

	Matrix3& Matrix3::FromQuaternion(const Quaternion& quat)
	{
		// calculate coefficients
		const float x2(quat.x + quat.x);
		const float y2(quat.y + quat.y);
		const float z2(quat.z + quat.z);

		const float xx2(quat.x * x2), xy2(quat.x * y2), xz2(quat.x * z2);
		const float yy2(quat.y * y2), yz2(quat.y * z2), zz2(quat.z * z2);
		const float wx2(quat.w * x2), wy2(quat.w * y2), wz2(quat.w * z2);

		m11 = 1 - yy2 - zz2; m12 = xy2 + wz2;		m13 = xz2 - wy2;	
		m21 = xy2 - wz2;	 m22 = 1 - xx2 - zz2;	m23 = yz2 + wx2;	
		m31 = xz2 + wy2,	 m32 = yz2 - wx2;		m33 = 1 - xx2 - yy2;
		
		return *this;
	}
}