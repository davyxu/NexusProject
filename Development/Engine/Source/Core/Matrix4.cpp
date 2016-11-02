#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	const Matrix4 Matrix4::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	const Matrix4 Matrix4::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );


	bool Matrix4::operator == ( const Matrix4& matrix)
	{
		return ::memcmp(this->m,matrix.m,sizeof(Matrix4)) == 0;
	}

	bool Matrix4::operator != ( const Matrix4& matrix)
	{
		return ::memcmp(this->m,matrix.m,sizeof(Matrix4)) != 0;
	}

	Matrix4& Matrix4::operator += ( const Matrix4& matrix )
	{
		m[0][0] += matrix.m[0][0]; m[0][1] += matrix.m[0][1]; m[0][2] += matrix.m[0][2]; m[0][3] += matrix.m[0][3];
		m[1][0] += matrix.m[1][0]; m[1][1] += matrix.m[1][1]; m[1][2] += matrix.m[1][2]; m[1][3] += matrix.m[1][3];
		m[2][0] += matrix.m[2][0]; m[2][1] += matrix.m[2][1]; m[2][2] += matrix.m[2][2]; m[2][3] += matrix.m[2][3];
		m[3][0] += matrix.m[3][0]; m[3][1] += matrix.m[3][1]; m[3][2] += matrix.m[3][2]; m[3][3] += matrix.m[3][3];

		return *this;
	}

	Matrix4& Matrix4::operator -= ( const Matrix4& matrix )
	{
		m[0][0] -= matrix.m[0][0]; m[0][1] -= matrix.m[0][1]; m[0][2] -= matrix.m[0][2]; m[0][3] -= matrix.m[0][3];
		m[1][0] -= matrix.m[1][0]; m[1][1] -= matrix.m[1][1]; m[1][2] -= matrix.m[1][2]; m[1][3] -= matrix.m[1][3];
		m[2][0] -= matrix.m[2][0]; m[2][1] -= matrix.m[2][1]; m[2][2] -= matrix.m[2][2]; m[2][3] -= matrix.m[2][3];
		m[3][0] -= matrix.m[3][0]; m[3][1] -= matrix.m[3][1]; m[3][2] -= matrix.m[3][2]; m[3][3] -= matrix.m[3][3];

		return *this;
	}

	Matrix4& Matrix4::operator *= ( const Matrix4& matrix )
	{
		float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3], m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3],
			m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3], m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		m[0][0] = m00 * matrix.m[0][0] + m01 * matrix.m[1][0] + m02 * matrix.m[2][0] + m03 * matrix.m[3][0];
		m[0][1] = m00 * matrix.m[0][1] + m01 * matrix.m[1][1] + m02 * matrix.m[2][1] + m03 * matrix.m[3][1];
		m[0][2] = m00 * matrix.m[0][2] + m01 * matrix.m[1][2] + m02 * matrix.m[2][2] + m03 * matrix.m[3][2];
		m[0][3] = m00 * matrix.m[0][3] + m01 * matrix.m[1][3] + m02 * matrix.m[2][3] + m03 * matrix.m[3][3];

		m[1][0] = m10 * matrix.m[0][0] + m11 * matrix.m[1][0] + m12 * matrix.m[2][0] + m13 * matrix.m[3][0];
		m[1][1] = m10 * matrix.m[0][1] + m11 * matrix.m[1][1] + m12 * matrix.m[2][1] + m13 * matrix.m[3][1];
		m[1][2] = m10 * matrix.m[0][2] + m11 * matrix.m[1][2] + m12 * matrix.m[2][2] + m13 * matrix.m[3][2];
		m[1][3] = m10 * matrix.m[0][3] + m11 * matrix.m[1][3] + m12 * matrix.m[2][3] + m13 * matrix.m[3][3];

		m[2][0] = m20 * matrix.m[0][0] + m21 * matrix.m[1][0] + m22 * matrix.m[2][0] + m23 * matrix.m[3][0];
		m[2][1] = m20 * matrix.m[0][1] + m21 * matrix.m[1][1] + m22 * matrix.m[2][1] + m23 * matrix.m[3][1];
		m[2][2] = m20 * matrix.m[0][2] + m21 * matrix.m[1][2] + m22 * matrix.m[2][2] + m23 * matrix.m[3][2];
		m[2][3] = m20 * matrix.m[0][3] + m21 * matrix.m[1][3] + m22 * matrix.m[2][3] + m23 * matrix.m[3][3];

		m[3][0] = m30 * matrix.m[0][0] + m31 * matrix.m[1][0] + m32 * matrix.m[2][0] + m33 * matrix.m[3][0];
		m[3][1] = m30 * matrix.m[0][1] + m31 * matrix.m[1][1] + m32 * matrix.m[2][1] + m33 * matrix.m[3][1];
		m[3][2] = m30 * matrix.m[0][2] + m31 * matrix.m[1][2] + m32 * matrix.m[2][2] + m33 * matrix.m[3][2];
		m[3][3] = m30 * matrix.m[0][3] + m31 * matrix.m[1][3] + m32 * matrix.m[2][3] + m33 * matrix.m[3][3];

		return *this;
	}

	Matrix4& Matrix4::operator *= ( float scalar )
	{
		m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar; m[0][3] *= scalar;
		m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar; m[1][3] *= scalar;
		m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar; m[2][3] *= scalar;
		m[3][0] *= scalar; m[3][1] *= scalar; m[3][2] *= scalar; m[3][3] *= scalar;

		return *this;
	}

	Matrix4& Matrix4::operator /= ( float scalar )
	{
		m[0][0] /= scalar; m[0][1] /= scalar; m[0][2] /= scalar; m[0][3] /= scalar;
		m[1][0] /= scalar; m[1][1] /= scalar; m[1][2] /= scalar; m[1][3] /= scalar;
		m[2][0] /= scalar; m[2][1] /= scalar; m[2][2] /= scalar; m[2][3] /= scalar;
		m[3][0] /= scalar; m[3][1] /= scalar; m[3][2] /= scalar; m[3][3] /= scalar;

		return *this;
	}

	Matrix4 Matrix4::operator + ( const Matrix4& matrix ) const
	{
		Matrix4 returnmatrix = *this;

		return returnmatrix += matrix;
	}

	Matrix4 Matrix4::operator - ( const Matrix4& matrix ) const
	{
		Matrix4 returnmatrix = *this;

		return returnmatrix -= matrix;
	}

	Matrix4 Matrix4::operator * ( const Matrix4& matrix ) const
	{
		Matrix4 returnmatrix = *this;

		return returnmatrix *= matrix;
	}

	Matrix4 Matrix4::operator * ( float scalar ) const
	{
		Matrix4 returnmatrix = *this;

		return returnmatrix *= scalar;
	}

	Matrix4 Matrix4::operator / ( float scalar ) const
	{
		Matrix4 returnmatrix = *this;

		return returnmatrix /= scalar;
	}

	Matrix4& Matrix4::FromQuaternion(const Quaternion& quat)
	{
		// calculate coefficients
		const float x2(quat.x + quat.x);
		const float y2(quat.y + quat.y);
		const float z2(quat.z + quat.z);

		const float xx2(quat.x * x2), xy2(quat.x * y2), xz2(quat.x * z2);
		const float yy2(quat.y * y2), yz2(quat.y * z2), zz2(quat.z * z2);
		const float wx2(quat.w * x2), wy2(quat.w * y2), wz2(quat.w * z2);

		m11 = 1 - yy2 - zz2; m12 = xy2 + wz2;		m13 = xz2 - wy2;		m14 = 0;
		m21 = xy2 - wz2;	 m22 = 1 - xx2 - zz2;	m23 = yz2 + wx2;		m24 = 0;
		m31 = xz2 + wy2,	 m32 = yz2 - wx2;		m33 = 1 - xx2 - yy2;	m34 = 0;
		m41 = 0;			 m42 = 0;				m43 = 0;				m44 = 1;

		return *this;
	}

	Matrix4 Matrix4::Identity( ) const
	{
		return Matrix4::cIdentity;
	}

	Matrix4 Matrix4::Transpose( ) const
	{
		return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	Matrix4 Matrix4::Inverse( ) const
	{
		float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
		float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
		float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
		float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;

		float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

		float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		float d00 = t00 * invDet;
		float d10 = t10 * invDet;
		float d20 = t20 * invDet;
		float d30 = t30 * invDet;

		float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return Matrix4(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}

	Matrix4& Matrix4::Translation( float x, float y, float z )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] =    x; m[3][1] =    y; m[3][2] =    z; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::SetTranslation( const Vector3& vector )
	{
		m[3][0] =  vector.x; m[3][1] =    vector.y; m[3][2] =    vector.z; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::Translation( const Vector3& vector )
	{
		return Translation( vector.x, vector.y, vector.z );
	}

	Matrix4& Matrix4::Scale( float x, float y, float z )
	{
		m[0][0]	=    x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] =    y; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] =    z; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::Scale( const Vector3& vector )
	{
		return Scale( vector.x, vector.y, vector.z );
	}

	Matrix4& Matrix4::Scale( const Vector3& vector, float factor )
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

		m[0][0]	= 1.0f + k * xx; m[0][1] =        k * xy; m[0][2] =        k * zx; m[0][3] = 0.0f;
		m[1][0]	=        k * xy; m[1][1] = 1.0f + k * yy; m[1][2] =        k * yz; m[1][3] = 0.0f;
		m[2][0]	=        k * zx; m[2][1] =        k * yz; m[2][2] = 1.0f + k * zz; m[2][3] = 0.0f;
		m[3][0] =          0.0f; m[3][1] =          0.0f; m[3][2] =          0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::RotationX( float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

		m[0][0]	= 1.0f; m[0][1] =       0.0f; m[0][2] =     0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] =   cosvalue; m[1][2] = sinvalue; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = - sinvalue; m[2][2] = cosvalue; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] =       0.0f; m[3][2] =     0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::RotationY( float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

		m[0][0]	= cosvalue; m[0][1] = 0.0f; m[0][2] = - sinvalue; m[0][3] = 0.0f;
		m[1][0] =     0.0f; m[1][1] = 1.0f; m[1][2] =       0.0f; m[1][3] = 0.0f;
		m[2][0] = sinvalue; m[2][1] = 0.0f; m[2][2] =   cosvalue; m[2][3] = 0.0f;
		m[3][0] =     0.0f; m[3][1] = 0.0f; m[3][2] =       0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::RotationZ( float radian )
	{
		float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

		m[0][0]	=   cosvalue; m[0][1] = sinvalue; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = - sinvalue; m[1][1] = cosvalue; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] =       0.0f; m[2][1] =     0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] =       0.0f; m[3][1] =     0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::RotationAxis( const Vector3& axis, float radian )
	{
		Quaternion quat;
		quat.RotationAxis(axis, radian);
		FromQuaternion(quat);
		return *this;
	}

	Matrix4& Matrix4::Rotation( const Vector3& srcvector, const Vector3& desvector )
	{
		Quaternion rotation;
		rotation.Rotation( srcvector, desvector );

		FromTranslationRotation( Vector3::cZero, rotation );

		return *this;
	}

	Matrix4& Matrix4::Rotation( const Quaternion& rotation )
	{
		float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
		float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
		float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

		m[0][0] = 1.0f - yy - zz; m[0][1] =        xy + zw; m[0][2] =        xz - yw; m[0][3] = 0.0f;
		m[1][0] =        xy - zw; m[1][1] = 1.0f - xx - zz; m[1][2] =        yz + xw; m[1][3] = 0.0f;
		m[2][0] =        xz + yw; m[2][1] =        yz - xw; m[2][2] = 1.0f - xx - yy; m[2][3] = 0.0f;
		m[3][0] =           0.0f; m[3][1] =           0.0f; m[3][2] =           0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::ProjectXY( )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::ProjectYZ( )
	{
		m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::ProjectZX( )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::Project( const Vector3& vector )
	{
		return Scale( vector, 0.0f );
	}

	Matrix4& Matrix4::RefectionXY( )
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] =  0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] =  0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = -1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] =  0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::RefectionYZ( )
	{
		m[0][0] = -1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] =  0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] =  0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] =  0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::RefectionZX( )
	{
		m[0][0] = 1.0f; m[0][1] =  0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = -1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] =  0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] =  0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::Refection( const Vector3& vector )
	{
		return Scale( vector, -1.0f );
	}

	Matrix4& Matrix4::FromTranslationRotation( const Vector3& translation, const Matrix3& rotation )
	{
		m[0][0] = rotation.m[0][0]; m[0][1] = rotation.m[0][1]; m[0][2] = rotation.m[0][2]; m[0][3] = 0.0f;
		m[1][0] = rotation.m[1][0]; m[1][1] = rotation.m[1][1]; m[1][2] = rotation.m[1][2]; m[1][3] = 0.0f;
		m[2][0] = rotation.m[2][0]; m[2][1] = rotation.m[2][1]; m[2][2] = rotation.m[2][2]; m[2][3] = 0.0f;
		m[3][0] =    translation.x; m[3][1] =    translation.y; m[3][2] =    translation.z; m[3][3] = 1.0f;

		return *this;
	}

	void Matrix4::ToTranslationRotation( Vector3& translation, Matrix3& rotation ) const
	{
		translation.x = m[3][0];
		translation.y = m[3][1];
		translation.z = m[3][2];

		rotation.m[0][0] = m[0][0]; rotation.m[0][1] = m[0][1]; rotation.m[0][2] = m[0][2];
		rotation.m[1][0] = m[1][0]; rotation.m[1][1] = m[1][1]; rotation.m[1][2] = m[1][2];
		rotation.m[2][0] = m[2][0]; rotation.m[2][1] = m[2][1]; rotation.m[2][2] = m[2][2];
	}

	Matrix4& Matrix4::FromTranslationRotation( const Vector3& translation, const Quaternion& rotation )
	{
		float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
		float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
		float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

		m[0][0] = 1.0f - yy - zz; m[0][1] =        xy + zw; m[0][2] =        xz - yw; m[0][3] = 0.0f;
		m[1][0] =        xy - zw; m[1][1] = 1.0f - xx - zz; m[1][2] =        yz + xw; m[1][3] = 0.0f;
		m[2][0] =        xz + yw; m[2][1] =        yz - xw; m[2][2] = 1.0f - xx - yy; m[2][3] = 0.0f;
		m[3][0] =  translation.x; m[3][1] =  translation.y; m[3][2] =  translation.z; m[3][3] = 1.0f;

		return *this;
	}

	Matrix4& Matrix4::FromTransform( const Vector3& translation, const Quaternion& rotation, const Vector3& scaling  )
	{
		return FromMatrix3( Matrix3().FromQuaternion( rotation ) *
							Matrix3().Scaling( scaling ) ).
			   SetTranslation( translation );
	}

	Matrix4& Matrix4::FromMatrix3( const Matrix3& Mat  )
	{
		m[0][0] = Mat.m[0][0]; m[0][1] = Mat.m[0][1]; m[0][2] = Mat.m[0][2];  m[0][3] = 0.0f;
		m[1][0] = Mat.m[1][0]; m[1][1] = Mat.m[1][1]; m[1][2] = Mat.m[1][2];  m[1][3] = 0.0f;
		m[2][0] = Mat.m[2][0]; m[2][1] = Mat.m[2][1]; m[2][2] = Mat.m[2][2];  m[2][3] = 0.0f;

		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}


	void Matrix4::ToTranslationRotation( Vector3& translation, Quaternion& rotation ) const
	{
		translation.x = m[3][0];
		translation.y = m[3][1];
		translation.z = m[3][2];

		// Determine which of w, x, y, or z has the largest absolute value.
		float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];
		float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];

		int biggestIndex = 0;
		float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

		if ( fourXSquaredMinus1 > fourBiggestSquaredMinus1 )
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}

		if ( fourYSquaredMinus1 > fourBiggestSquaredMinus1 )
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}

		if ( fourZSquaredMinus1 > fourBiggestSquaredMinus1 )
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}


		float biggestVal = Math::Sqrt( fourBiggestSquaredMinus1 + 1.0f ) * 0.5f;
		float mult = 0.25f / biggestVal;

		// Apply table to compute quaternion values.
		switch ( biggestIndex )
		{
		case 0:
			rotation.w = biggestVal;
			rotation.x = ( m[1][2] - m[2][1] ) * mult;
			rotation.y = ( m[2][0] - m[0][2] ) * mult;
			rotation.z = ( m[0][1] - m[1][0] ) * mult;
			break;

		case 1:
			rotation.x = biggestVal;
			rotation.w = ( m[1][2] - m[2][1] ) * mult;
			rotation.y = ( m[0][1] + m[1][0] ) * mult;
			rotation.z = ( m[2][0] + m[0][2] ) * mult;
			break;

		case 2:
			rotation.y = biggestVal;
			rotation.w = ( m[2][0] - m[0][2] ) * mult;
			rotation.x = ( m[0][1] + m[1][0] ) * mult;
			rotation.z = ( m[1][2] + m[2][1] ) * mult;
			break;

		case 3:
			rotation.z = biggestVal;
			rotation.w = ( m[0][1] - m[1][0] ) * mult;
			rotation.x = ( m[2][0] + m[0][2] ) * mult;
			rotation.y = ( m[1][2] + m[2][1] ) * mult;
			break;
		}
	}

	Matrix4 Matrix4::OrthoLH( float width, float height, float znear, float zfar )
	{
		float xs = 2.0f / width;
		float ys = 2.0f / height;
		float zf = 1.0f / ( zfar - znear );
		float zn = - znear * zf;

		return Matrix4(
			xs, 0.0f, 0.0f, 0.0f,
			0.0f,   ys, 0.0f, 0.0f,
			0.0f, 0.0f,   zf, 0.0f,
			0.0f, 0.0f,   zn, 1.0f );
	}


	Matrix4 Matrix4::OrthoOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar )
	{
		// OpenGL的屏幕坐标原点为左下角，对应这个矩阵就是正确的
		// D3D则要自己反向下

		float xs1 = 2.0f / ( right - left );
		float xs2 = ( left + right ) / ( left - right );
		float ys1 = 2.0f / ( top - bottom );
		float ys2 = ( bottom + top ) / ( bottom - top );
		float zf  = 1.0f / ( zfar - znear );
		float zn = znear / ( znear - zfar);

		return Matrix4(
			xs1, 0.0f, 0.0f, 0.0f,
			0.0f,  ys1, 0.0f, 0.0f,
			0.0f, 0.0f,   zf, 0.0f,
			xs2,  ys2,   zn, 1.0f );
	}

	Matrix4 Matrix4::OrthoRH( float width, float height, float znear, float zfar )
	{
		float xs = 2.0f / width;
		float ys = 2.0f / height;
		float zf = 1.0f / ( znear - zfar );
		float zn = znear * zf;

		return Matrix4(
			xs, 0.0f, 0.0f, 0.0f,
			0.0f,   ys, 0.0f, 0.0f,
			0.0f, 0.0f,   zf, 0.0f,
			0.0f, 0.0f,   zn, 1.0f );
	}

	Matrix4 Matrix4::OrthoOffCenterRH( float left, float right, float bottom, float top, float znear, float zfar )
	{
		float xs1 = 2.0f / ( right - left );
		float xs2 = ( left + right ) / ( left - right );
		float ys1 = 2.0f / ( top - bottom );
		float ys2 = ( bottom + top ) / ( bottom - top );
		float zf  = 1.0f / ( znear - zfar );
		float zn  = znear * zf;

		return Matrix4(
			xs1, 0.0f, 0.0f, 0.0f,
			0.0f,  ys1, 0.0f, 0.0f,
			0.0f, 0.0f,   zf, 0.0f,
			xs2,  ys2,   zn, 1.0f );
	}

	Matrix4 Matrix4::PerspectiveLH( float width, float height, float znear, float zfar )
	{
		float xs = 2.0f * znear / width;
		float ys = 2.0f * znear / height;
		float zf = zfar / ( zfar - znear );
		float zn = - znear * zf;

		return Matrix4(
			xs, 0.0f, 0.0f, 0.0f,
			0.0f,   ys, 0.0f, 0.0f,
			0.0f, 0.0f,   zf, 1.0f,
			0.0f, 0.0f,   zn, 0.0f );
	}

	Matrix4 Matrix4::PerspectiveFovLH( float fovy, float aspect, float znear, float zfar )
	{
		float ys = Math::Cot( fovy / 2.0f );
		float xs = ys / aspect;
		float zf = zfar / ( zfar - znear );
		float zn = - znear * zf;

		return Matrix4(
			xs, 0.0f, 0.0f, 0.0f,
			0.0f,   ys, 0.0f, 0.0f,
			0.0f, 0.0f,   zf, 1.0f,
			0.0f, 0.0f,   zn, 0.0f );
	}

	Matrix4 Matrix4::PerspectiveOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar )
	{
		float xs1 = 2.0f * znear / ( right - left );
		float xs2 = ( left + right ) / ( left - right );
		float ys1 = 2.0f * znear / ( top - bottom );
		float ys2 = ( bottom + top ) / ( bottom - top );
		float zf  = zfar / ( zfar - znear );
		float zn  = - znear * zf;

		return Matrix4(
			xs1, 0.0f, 0.0f, 0.0f,
			0.0f,  ys1, 0.0f, 0.0f,
			xs2,  ys2,   zf, 1.0f,
			0.0f, 0.0f,   zn, 0.0f );
	}

	Matrix4 Matrix4::PerspectiveRH( float width, float height, float znear, float zfar )
	{
		float xs = 2.0f * znear / width;
		float ys = 2.0f * znear / height;
		float zf = zfar / ( znear - zfar );
		float zn = znear * zf;

		return Matrix4(
			xs, 0.0f, 0.0f,  0.0f,
			0.0f,   ys, 0.0f,  0.0f,
			0.0f, 0.0f,   zf, -1.0f,
			0.0f, 0.0f,   zn,  0.0f );
	}

	Matrix4 Matrix4::PerspectiveFovRH( float fovy, float aspect, float znear, float zfar )
	{
		float ys = Math::Cot( fovy / 2.0f );
		float xs = ys / aspect;
		float zf = zfar / ( znear - zfar );
		float zn = znear * zf;

		return Matrix4(
			xs, 0.0f, 0.0f,  0.0f,
			0.0f,   ys, 0.0f,  0.0f,
			0.0f, 0.0f,   zf, -1.0f,
			0.0f, 0.0f,   zn,  0.0f );
	}

	Matrix4 Matrix4::PerspectiveOffCenterRH( float left, float right, float bottom, float top, float znear, float zfar )
	{
		float xs1 = 2.0f * znear / ( right - left );
		float xs2 = ( left + right ) / ( right - left );
		float ys1 = 2.0f * znear / ( top - bottom );
		float ys2 = ( bottom + top ) / ( top - bottom );
		float zf = zfar / ( znear - zfar );
		float zn  = znear * zf;

		return Matrix4(
			xs1, 0.0f, 0.0f,  0.0f,
			0.0f,  ys1, 0.0f,  0.0f,
			xs2,  ys2,   zf, -1.0f,
			0.0f, 0.0f,   zn,  0.0f );
	}

	Matrix4 Matrix4::LookAtLH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis )
	{
		Vector3 zaxis = ( lookat - eye ).Normalize( );
		Vector3 xaxis = Vector3::CrossProduct( upaxis, zaxis ).Normalize( );
		Vector3 yaxis = Vector3::CrossProduct( zaxis, xaxis );

		float xeye = - Vector3::DotProduct( xaxis, eye );
		float yeye = - Vector3::DotProduct( yaxis, eye );
		float zeye = - Vector3::DotProduct( zaxis, eye );

		return Matrix4(
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			xeye,    yeye,    zeye, 1.0f );
	}

	Matrix4 Matrix4::LookAtRH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis )
	{
		Vector3 zaxis = ( eye - lookat ).Normalize( );
		Vector3 xaxis = Vector3::CrossProduct( upaxis, zaxis ).Normalize( );
		Vector3 yaxis = Vector3::CrossProduct( zaxis, xaxis );

		float xeye = - Vector3::DotProduct( xaxis, eye );
		float yeye = - Vector3::DotProduct( yaxis, eye );
		float zeye = - Vector3::DotProduct( zaxis, eye );

		return Matrix4(
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			xeye,    yeye,    zeye, 1.0f );
	}


	Vector4 Matrix4::TransformNormal( const Vector3& v )
	{
		return TransformCoord( Vector4(v.x, v.y, v.z, 0.0f)  );
	}

	Vector4 Matrix4::TransformCoord( const Vector4& v )
	{
		float xx = v.x, yy = v.y, zz = v.z, ww = v.w ;

		Vector4 Result;
		Result.x = xx * m11 + yy * m21 + zz * m31+ ww * m41; 
		Result.y =	xx * m12 + yy * m22 + zz * m32+ ww * m42;
		Result.z =	xx * m13 + yy * m23 + zz * m33+ ww * m43;
		Result.w =	xx * m14 + yy * m24 + zz * m34+ ww * m44;

		return Result;
	}

	Matrix4& Matrix4::FromRotator( const Rotator& Rot )
	{
		const FLOAT	SR	= MathTable::SinTab(Rot.Roll);
		const FLOAT	SP	= MathTable::SinTab(Rot.Pitch);
		const FLOAT	SY	= MathTable::SinTab(Rot.Yaw);
		const FLOAT	CR	= MathTable::CosTab(Rot.Roll);
		const FLOAT	CP	= MathTable::CosTab(Rot.Pitch);
		const FLOAT	CY	= MathTable::CosTab(Rot.Yaw);

		m[0][0]	= CP * CY;
		m[0][1]	= CP * SY;
		m[0][2]	= SP;
		m[0][3]	= 0.f;

		m[1][0]	= SR * SP * CY - CR * SY;
		m[1][1]	= SR * SP * SY + CR * CY;
		m[1][2]	= - SR * CP;
		m[1][3]	= 0.f;

		m[2][0]	= -( CR * SP * CY + SR * SY );
		m[2][1]	= CY * SR - CR * SP * SY;
		m[2][2]	= CR * CP;
		m[2][3]	= 0.f;

		m[3][0]	= 0.f;
		m[3][1]	= 0.f;
		m[3][2]	= 0.f;
		m[3][3]	= 1.f;

		return *this;
	}

	Rotator Matrix4::GetRotator() const
	{
		const Vector3		XAxis	= GetRow( 0 );
		const Vector3		YAxis	= GetRow( 1 );
		const Vector3		ZAxis	= GetRow( 2 );

		Rotator	Rot	= Rotator( 
			Math::Round(Math::ArcTan2( XAxis.z, Math::Sqrt(Math::Square(XAxis.x)+Math::Square(XAxis.y)) ) * 32768.f / Math::cPi), 
			Math::Round(Math::ArcTan2( XAxis.y, XAxis.x ) * 32768.f / Math::cPi), 
			0 
			);

		const Vector3		SYAxis	= Matrix4().FromRotator( Rot ).GetAxis(1);
		Rot.Roll		= Math::Round(Math::ArcTan2( ZAxis | SYAxis, YAxis | SYAxis ) * 32768.f / Math::cPi);
		return Rot;
	}

	
}