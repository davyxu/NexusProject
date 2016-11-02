#include "CorePCH.h"
#include "NekoCore_Math.h"


namespace NekoEngine
{
	const Quaternion Quaternion::cIdentity( 0.0f, 0.0f, 0.0f, 1.0f );

	const Quaternion Quaternion::cZero( 0.0f, 0.0f, 0.0f, 0.0f );

	Quaternion& Quaternion::operator *= ( const Quaternion& quaternion )
	{
		float qx1 = x, qy1 = y, qz1 = z, qw1 = w, qx2 = quaternion.x, qy2 = quaternion.y,
			qz2 = quaternion.z, qw2 = quaternion.w;

		x = qw1 * qx2 + qx1 * qw2 + qy1 * qz2 - qz1 * qy2;
		y = qw1 * qy2 - qx1 * qz2 + qy1 * qw2 + qz1 * qx2;
		z = qw1 * qz2 + qx1 * qy2 - qy1 * qx2 + qz1 * qw2;
		w = qw1 * qw2 - qx1 * qx2 - qy1 * qy2 - qz1 * qz2;

		return *this;
	}

	Quaternion& Quaternion::operator /= ( const Quaternion& quaternion )
	{
		Quaternion tempquaternion = *this;
		*this = -quaternion;

		return *this *= tempquaternion;
	}

	Quaternion Quaternion::operator * ( const Quaternion& quaternion ) const
	{
		Quaternion returnquaternion = *this;

		return returnquaternion *= quaternion;
	}

	Quaternion Quaternion::operator / ( const Quaternion& quaternion ) const
	{
		Quaternion returnquaternion = *this;

		return returnquaternion /= quaternion;
	}

	Vector3 Quaternion::operator* (const Vector3& v) const
	{
		// nVidia SDK implementation
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = Vector3::CrossProduct(qvec, v );
		uuv = Vector3::CrossProduct(qvec,uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return v + uv + uuv;

	}

	bool Quaternion::operator == ( const Quaternion& quaternion) const
	{
		const float margin = Math::cEpsilon;

		if ( Math::Abs( x - quaternion.x ) >  margin )
			return false;

		if ( Math::Abs( y - quaternion.y ) >  margin )
			return false;

		if ( Math::Abs( z - quaternion.z ) >  margin )
			return false;

		if ( Math::Abs( w - quaternion.w ) >  margin )
			return false;

		return true;
	}


	float Quaternion::operator|( const Quaternion& v ) const
	{
		return v.x * x + v.y * y + v.z * z + v.w * w;
	}


	Quaternion& Quaternion::Identity( )
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;

		return *this;
	}

	Quaternion& Quaternion::Normalize( )
	{
		float mag = Magnitude( );

		if ( mag > Math::cEpsilon )
		{
			float magdiv = 1.0f / mag;
			x *= magdiv;
			y *= magdiv;
			z *= magdiv;
		}

		return *this;
	}

	Quaternion Quaternion::Inverse () const
	{
		float fNorm = w*w+x*x+y*y+z*z;
		if ( fNorm > 0.0 )
		{
			float fInvNorm = 1.0f/fNorm;
			return Quaternion(-x*fInvNorm,-y*fInvNorm,-z*fInvNorm, w*fInvNorm );
		}
		else
		{
			// return an invalid result to flag the error
			return Quaternion::cZero;
		}
	}

	float Quaternion::Magnitude( ) const
	{
		return Math::Sqrt( x * x + y * y + z * z + w * w );
	}

	float Quaternion::MagnitudeSquared( ) const
	{
		return x * x + y * y + z * z + w * w;
	}

	Quaternion& Quaternion::RotationX( float radian )
	{
		x = Math::Sin( radian * 0.5f );
		y = 0.0f;
		z = 0.0f;
		w = Math::Cos( radian * 0.5f );

		return *this;
	}

	Quaternion& Quaternion::RotationY( float radian )
	{
		x = 0.0f;
		y = Math::Sin( radian * 0.5f );
		z = 0.0f;
		w = Math::Cos( radian * 0.5f );

		return *this;
	}

	Quaternion& Quaternion::RotationZ( float radian )
	{
		x = 0.0f;
		y = 0.0f;
		z = Math::Sin( radian * 0.5f );
		w = Math::Cos( radian * 0.5f );

		return *this;
	}

	Quaternion& Quaternion::RotationAxis( const Vector3& axis, float radian )
	{
		float sinvalue = Math::Sin( radian * 0.5f );

		x = sinvalue * axis.x;
		y = sinvalue * axis.y;
		z = sinvalue * axis.z;
		w = Math::Cos( radian * 0.5f );

		return *this;
	}

	Quaternion& Quaternion::Rotation( const Vector3& srcvector, const Vector3& desvector )
	{
		Vector3 vector1 = srcvector;
		vector1.Normalize( );

		Vector3 vector2 = desvector;
		vector2.Normalize( );

		float dotvalue = Vector3::DotProduct( vector1, vector2 );
		if ( dotvalue >= 1.0f )
			return ( *this = Quaternion::cIdentity );

		float sqrtvalue = Math::Sqrt( ( 1.0f + dotvalue ) * 2.0f );
		if ( sqrtvalue < Math::cEpsilon )
			return ( *this = Quaternion::cIdentity );

		Vector3 axis = Vector3::CrossProduct( vector1, vector2 );

		x = axis.x / sqrtvalue;
		y = axis.y / sqrtvalue;
		z = axis.z / sqrtvalue;
		w = sqrtvalue * 0.5f;

		return *this;
	}

	void Quaternion::GetAxisAndAngle( Vector3& axis, float& radian ) const
	{
		float length = x * x + y * y + z * z;

		if ( length > Math::cEpsilon )
		{
			length = Math::Sqrt( length );

			axis.x = x / length;
			axis.y = y / length;
			axis.z = z / length;
			radian = 2.0f * Math::ArcCos( w );
		}
		else
		{
			axis.x = 0.0f;
			axis.y = 0.0f;
			axis.z = 1.0f;
			radian = 0.0f;
		}

		axis.Normalize( );
	}

	Quaternion& Quaternion::FromAxes (const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
	{
		Matrix4 kRot;

		kRot.m[0][0] = xaxis.x;
		kRot.m[1][0] = xaxis.y;
		kRot.m[2][0] = xaxis.z;

		kRot.m[0][1] = yaxis.x;
		kRot.m[1][1] = yaxis.y;
		kRot.m[2][1] = yaxis.z;

		kRot.m[0][2] = zaxis.x;
		kRot.m[1][2] = zaxis.y;
		kRot.m[2][2] = zaxis.z;

		FromMatrix(kRot);

		return *this;
	}

	Quaternion& Quaternion::FromMatrix( const Matrix4& m )
	{
		//const MeReal *const t = (MeReal *) tm;
		float	s;

		// Check diagonal (trace)
		const float tr = m.m[0][0] + m.m[1][1] + m.m[2][2];

		if (tr > 0.0f) 
		{
			float InvS = Math::InvSqrt(tr + 1.f);
			w = 0.5f * (1.f / InvS);
			s = 0.5f * InvS;

			x = (m.m[1][2] - m.m[2][1]) * s;
			y = (m.m[2][0] - m.m[0][2]) * s;
			z = (m.m[0][1] - m.m[1][0]) * s;
		} 
		else 
		{
			// diagonal is negative
			INT i = 0;

			if (m.m[1][1] > m.m[0][0])
				i = 1;

			if (m.m[2][2] > m.m[i][i])
				i = 2;

			static const INT nxt[3] = { 1, 2, 0 };
			const INT j = nxt[i];
			const INT k = nxt[j];

			s = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;

			float InvS = Math::InvSqrt(s);

			float qt[4];
			qt[i] = 0.5f * (1.f / InvS);

			s = 0.5f * InvS;

			qt[3] = (m.m[j][k] - m.m[k][j]) * s;
			qt[j] = (m.m[i][j] + m.m[j][i]) * s;
			qt[k] = (m.m[i][k] + m.m[k][i]) * s;

			x = qt[0];
			y = qt[1];
			z = qt[2];
			w = qt[3];
		}

		return *this;
	}

	Vector3 Quaternion::GetXAxis( ) const
	{
		//float fTx  = 2.0*x;
		float fTy  = 2.0f*y;
		float fTz  = 2.0f*z;
		float fTwy = fTy*w;
		float fTwz = fTz*w;
		float fTxy = fTy*x;
		float fTxz = fTz*x;
		float fTyy = fTy*y;
		float fTzz = fTz*z;

		return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
	}
	//-----------------------------------------------------------------------
	Vector3 Quaternion::GetYAxis( ) const
	{
		float fTx  = 2.0f*x;
		float fTy  = 2.0f*y;
		float fTz  = 2.0f*z;
		float fTwx = fTx*w;
		float fTwz = fTz*w;
		float fTxx = fTx*x;
		float fTxy = fTy*x;
		float fTyz = fTz*y;
		float fTzz = fTz*z;

		return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
	}
	//-----------------------------------------------------------------------
	Vector3 Quaternion::GetZAxis( ) const
	{
		float fTx  = 2.0f*x;
		float fTy  = 2.0f*y;
		float fTz  = 2.0f*z;
		float fTwx = fTx*w;
		float fTwy = fTy*w;
		float fTxx = fTx*x;
		float fTxz = fTz*x;
		float fTyy = fTy*y;
		float fTyz = fTz*y;

		return Vector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
	}

	float Quaternion::DotProduct( const Quaternion& quaternion1, const Quaternion& quaternion2 )
	{
		return quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y +
			quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;
	}

	Quaternion Quaternion::Slerp( const Quaternion& quaternion1, const Quaternion& quaternion2, float factor )
	{
		factor = Math::Clamp( factor, 0.0f, 1.0f );

		float dotvalue = DotProduct( quaternion1, quaternion2 );

		bool flip = dotvalue < 0.0f;

		if ( dotvalue < 0.0f )
			dotvalue = -dotvalue;

		float divide = 0.0f;

		if ( 1.0f - dotvalue < Math::cEpsilon )
		{
			divide = 1.0f - factor;
		}
		else
		{
			float t = Math::ArcCos( dotvalue );
			float s = 1.0f / Math::Sin( t );

			divide	= Math::Sin( ( 1.0f - factor ) * t ) * s;
			factor	= Math::Sin( factor * t ) * s;
		}

		if ( flip )
			factor = -factor;

		return Quaternion(
			quaternion1.x * divide + quaternion2.x * factor,
			quaternion1.y * divide + quaternion2.y * factor,
			quaternion1.z * divide + quaternion2.z * factor,
			quaternion1.w * divide + quaternion2.w * factor );
	}


}