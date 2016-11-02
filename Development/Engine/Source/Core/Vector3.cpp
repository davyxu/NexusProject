#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	//Vector3
	//////////////////////////////////////////////////////////////////////////
	const Vector3 Vector3::cZero(0, 0, 0);
	const Vector3 Vector3::cOne(1, 1, 1);
	const Vector3 Vector3::cXAxis(1, 0, 0);
	const Vector3 Vector3::cYAxis(0, 1, 0);
	const Vector3 Vector3::cZAxis(0, 0, 1);

	Vector3::Vector3(void)
	{

	}

	Vector3::Vector3(const Vector3 &v)
		: x(v.x), y(v.y), z(v.z)
	{

	}

	Vector3::Vector3(float nx, float ny, float nz)
		: x(nx), y(ny), z(nz)
	{

	}

	Vector3 Vector3::operator + ( ) const
	{
		return Vector3( x, y, z );
	}

	Vector3 Vector3::operator - ( ) const
	{
		return Vector3( -x, -y, -z );
	}

	Vector3& Vector3::operator += (const Vector3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vector3& Vector3::operator -= (const Vector3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vector3& Vector3::operator *= ( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector3& Vector3::operator *= ( const Vector3& vector )
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;

		return *this;
	}

	Vector3& Vector3::operator /= ( float scalar )
	{
		float oneOverV = 1.0f / scalar;
		x *= oneOverV;
		y *= oneOverV;
		z *= oneOverV;

		return *this;
	}

	Vector3& Vector3::operator /= ( const Vector3& vector )
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;

		return *this;
	}

	Vector3 Vector3::operator + ( const Vector3& vector ) const
	{
		return Vector3( x + vector.x, y + vector.y, z + vector.z );
	}

	Vector3 Vector3::operator - ( const Vector3& vector ) const
	{
		return Vector3( x - vector.x, y - vector.y, z - vector.z );
	}

	Vector3 Vector3::operator * ( float scalar ) const
	{
		return Vector3( x * scalar, y * scalar, z * scalar );
	}

	Vector3 Vector3::operator * ( const Vector3& vector ) const
	{
		return Vector3( x * vector.x, y * vector.y, z * vector.z );
	}

	Vector3 Vector3::operator / ( float scalar ) const
	{
		return Vector3( x / scalar, y / scalar, z / scalar );
	}

	Vector3 Vector3::operator / ( const Vector3& vector ) const
	{
		return Vector3( x / vector.x, y / vector.y, z / vector.z );
	}


	float Vector3::operator | ( const Vector3& v ) const
	{
		return v.x * x + v.y * y + v.z * z;
	}

	//赋值
	Vector3& Vector3::operator = (const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	bool Vector3::operator == (const Vector3 &v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool Vector3::operator != (const Vector3 &v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	bool Vector3::Equal( const Vector3& vector, float margin ) const
	{
		if ( Math::Abs( x - vector.x ) > margin )
			return false;

		if ( Math::Abs( y - vector.y ) > margin )
			return false;

		if ( Math::Abs( z - vector.z ) > margin )
			return false;

		return true;
	}

	float Vector3::Magnitude(void) const
	{
		return Math::Sqrt(x * x + y * y + z * z);
	}

	float Vector3::MagnitudeSquared(void) const
	{
		return x * x + y * y + z * z;
	}

	// 取3轴最大最小数
	float Vector3::GetMax(void) const
	{
		return Math::Max3(x,y,z);
	}

	float Vector3::GetMin(void) const
	{
		return Math::Min3(x,y,z);
	}

	Vector3 Vector3::operator * ( const Matrix4& matrix ) const
	{
		Vector3 returnvector( *this );

		return returnvector *= matrix;
	}

	Vector3 Vector3::operator * ( const Quaternion& quaternion ) const
	{
		Vector3 returnvector( *this );

		return returnvector *= quaternion;
	}

	Vector3& Vector3::operator *= ( const Matrix4& matrix )
	{
		float xx = x, yy = y, zz = z;

		float w = xx * matrix.m14 + yy * matrix.m24 + zz * matrix.m34 + matrix.m44;

		x = xx * matrix.m11 + yy * matrix.m21 + zz * matrix.m31 + matrix.m41;
		y = xx * matrix.m12 + yy * matrix.m22 + zz * matrix.m32 + matrix.m42;
		z = xx * matrix.m13 + yy * matrix.m23 + zz * matrix.m33 + matrix.m43;

		x /= w;
		y /= w;
		z /= w;

		return *this;
	}

	Vector3& Vector3::operator *= ( const Quaternion& quaternion )
	{
		Quaternion tempquaternion = quaternion;
		tempquaternion *= Quaternion( x, y, z, 0.0f );
		tempquaternion *= -quaternion;

		x = tempquaternion.x;
		y = tempquaternion.y;
		z = tempquaternion.z;

		return *this;
	}

	//标准化
	Vector3& Vector3::Normalize(void)
	{
		float magSq = x*x + y*y + z*z;
		if (magSq > 0.0f )
		{
			float oneOverMag = 1.0f / Math::Sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}

		return *this;
	}

	// 设外值最大数
	Vector3& Vector3::MakeMax(const Vector3& p)
	{
		x = Math::Max(x,p.x);
		y = Math::Max(y,p.y);
		z = Math::Max(z,p.z);
		return *this;
	}

	// 设外值最小数
	Vector3& Vector3::MakeMin(const Vector3& p)
	{
		x = Math::Min(x,p.x);
		y = Math::Min(y,p.y);
		z = Math::Min(z,p.z);
		return *this;
	}

	Vector3& Vector3::TransformCoord(const Vector3& v, const Matrix4 &mat)
	{
		x = v.x * mat.m11 + v.y * mat.m21 + v.z * mat.m31;
		y =	v.x * mat.m12 + v.y * mat.m22 + v.z * mat.m32;
		z =	v.x * mat.m13 + v.y * mat.m23 + v.z * mat.m33;
		return *this;
	}


	Vector3 Vector3::Perpendicular(void) const
	{
		static const float fSquareZero = float(1e-06 * 1e-06);

		Vector3 perp = CrossProduct(*this, cXAxis );

		// Check length
		if( perp.MagnitudeSquared() < fSquareZero )
		{
			/* This vector is the Y axis multiplied by a scalar, so we have 
			to use another axis.
			*/
			perp = CrossProduct(*this, cYAxis );
		}

		return perp;
	}

	Vector3 Vector3::RandomDeviant(float angle,const Vector3& up) const
	{
		Vector3 newUp;

		if (up == Vector3::cZero)
		{
			// Generate an up vector
			newUp = this->Perpendicular();
		}
		else
		{
			newUp = up;
		}

		// Rotate up vector by random amount around this
		Quaternion q;
		q.RotationAxis( *this,Math::RandomFloat(0.0f, 1.0f ) * Math::c2Pi);
		newUp = newUp * q;

		// Finally rotate this by given angle around randomised up
		q.RotationAxis( newUp,angle);
		return (*this) * q;
	}


	float Vector3::DotProduct(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector3 Vector3::CrossProduct(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	float Vector3::DistanceTo(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return Math::Sqrt(dx * dx + dy * dy + dz * dz);
	}


	float Vector3::DistanceToSqare(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return dx * dx + dy * dy +dz * dz;
	}

	Vector3 Vector3::Lerp(const Vector3& v1,const Vector3& v2,float s)
	{
		Vector3 out;
		out.x = (v2.x - v1.x) * s + v1.x;
		out.y = (v2.y - v1.y) * s + v1.y;
		out.z = (v2.z - v1.z) * s + v1.z;

		return out;
	}

	Vector3 Vector3::Project( const Vector3& vector1, const Vector3& vector2 )
	{
		Vector3 normal = vector2;
		normal.Normalize( );

		return normal * DotProduct( vector1, normal );
	}


	// Copy from OGRE 1.65

	void Vector3::BuildTangentSpaceVector(	const Vector3& position1
										 ,  const Vector3& position2
										 ,  const Vector3& position3
										 ,	float u1
										 ,  float v1
										 ,  float u2
										 ,  float v2
										 ,  float u3
										 ,  float v3
										 ,  Vector3& OutTangent
										 ,  Vector3& OutBinormal )
	{
		//side0 is the vector along one side of the triangle of vertices passed in, 
		//and side1 is the vector along another side. Taking the cross product of these returns the normal.
		Vector3 side0 = position1 - position2;
		Vector3 side1 = position3 - position1;
		//Calculate face normal
		Vector3 normal = Vector3::CrossProduct(side1 , side0);
		normal.Normalize();
		//Now we use a formula to calculate the tangent. 
		float deltaV0 = v1 - v2;
		float deltaV1 = v3 - v1;
		OutTangent = side0 * deltaV1 - side1 * deltaV0;
		OutTangent.Normalize();
		//Calculate binormal
		float deltaU0 = u1 - u2;
		float deltaU1 = u3 - u1;
		OutBinormal = side0 * deltaU1 - side1 * deltaU0;
		OutBinormal.Normalize();
		//Now, we take the cross product of the tangents to get a vector which 
		//should point in the same direction as our normal calculated above. 
		//If it points in the opposite direction (the dot product between the normals is less than zero), 
		//then we need to reverse the s and t tangents. 
		//This is because the triangle has been mirrored when going from tangent space to object space.
		//reverse tangents if necessary
		Vector3 tangentCross = Vector3::CrossProduct(OutTangent, OutBinormal);

		if (Vector3::DotProduct( tangentCross , normal ) < 0.0f)
		{
			OutTangent = -OutTangent;
			OutBinormal = -OutBinormal;
		}
	}






}