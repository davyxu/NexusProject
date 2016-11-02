#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/


	/** 
		矢量4
	*/
	class Vector4
	{
	public:

		float x,y,z,w;

		/// 零
		const static Vector4 cZero;

		/// 一
		const static Vector4 cOne;

		/// x=1
		const static Vector4 cXAxis;

		/// y=1
		const static Vector4 cYAxis;

		/// z=1
		const static Vector4 cZAxis;

		/// w=1
		const static Vector4 cWAxis;

		const static Vector4 cMaxVector;

		const static Vector4 cMinVector;

		/// 构造
		inline Vector4( );

		/// 赋值构造
		inline Vector4( float xx, float yy, float zz, float ww );

		/// 矢量w构造
		inline Vector4( const Vector3& vector, float ww );

		/// 转换
		inline operator Vector3 ( ) const;

		/// 加
		inline Vector4 operator + ( ) const;

		/// 减
		inline Vector4 operator - ( ) const;

		/// 自加
		inline Vector4& operator += ( const Vector4& vector );

		/// 自减
		inline Vector4& operator -= ( const Vector4& vector );
		
		/// 自乘
		inline Vector4& operator *= ( float scalar );

		/// 乘自身
		inline Vector4& operator *= ( const Vector4& vector );

		/// 自除
		inline Vector4& operator /= ( float scalar );

		/// 自除
		inline Vector4& operator /= ( const Vector4& vector );

		/// 乘矩阵
		Vector4& operator *= ( const Matrix4& matrix );

		/// 加
		inline Vector4 operator + ( const Vector4& vector ) const;

		/// 减
		inline Vector4 operator - ( const Vector4& vector ) const;

		/// 乘
		inline Vector4 operator * ( float scalar ) const;

		/// 乘
		inline Vector4 operator * ( const Vector4& vector ) const;

		/// 除
		inline Vector4 operator / ( float scalar ) const;

		/// 除
		inline Vector4 operator / ( const Vector4& vector ) const;

		/// 乘矩阵
		Vector4 operator * ( const Matrix4& matrix ) const;

		/// 相等
		bool operator == ( const Vector4& vector ) const;


		/// 绝对值
		Vector4& Absolute( );

		/// 单位化
		Vector4& Normalize( );

		/// 模
		float Magnitude( ) const;

		/// 不开方模
		float MagnitudeSquared( ) const;

		/// 点积
		static float DotProduct( const Vector4& vector1, const Vector4& vector2 );

		/// 投射
		static Vector4 Project( const Vector4& vector1, const Vector4& vector2 );

		/// 插值
		static Vector4 Lerp( const Vector4& vector1, const Vector4& vector2, float factor );
	};


	Vector4::Vector4( )
	{
	}

	Vector4::Vector4( float xx, float yy, float zz, float ww )
		: x( xx ), y( yy ), z( zz ), w( ww )
	{
	}

	Vector4::Vector4( const Vector3& vector, float ww )
		: x( vector.x ), y( vector.y ), z( vector.z ), w( ww )
	{
	}

	Vector4::operator Vector3 ( ) const
	{
		return Vector3( x, y, z );
	}

	Vector4 Vector4::operator + ( ) const
	{
		return Vector4( x, y, z, w );
	}

	Vector4 Vector4::operator - ( ) const
	{
		return Vector4( -x, -y, -z, -w );
	}

	Vector4& Vector4::operator += ( const Vector4& vector )
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;

		return *this;
	}

	Vector4& Vector4::operator -= ( const Vector4& vector )
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;

		return *this;
	}

	Vector4& Vector4::operator *= ( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	Vector4& Vector4::operator *= ( const Vector4& vector )
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		w *= vector.w;

		return *this;
	}

	Vector4& Vector4::operator /= ( float scalar )
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	Vector4& Vector4::operator /= ( const Vector4& vector )
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		w /= vector.w;

		return *this;
	}

	Vector4 Vector4::operator + ( const Vector4& vector ) const
	{
		return Vector4( x + vector.x, y + vector.y, z + vector.z, w + vector.w );
	}

	Vector4 Vector4::operator - ( const Vector4& vector ) const
	{
		return Vector4( x - vector.x, y - vector.y, z - vector.z, w - vector.w );
	}

	Vector4 Vector4::operator * ( float scalar ) const
	{
		return Vector4( x * scalar, y * scalar, z * scalar, w * scalar );
	}

	Vector4 Vector4::operator * ( const Vector4& vector ) const
	{
		return Vector4( x * vector.x, y * vector.y, z * vector.z, w * vector.w );
	}

	Vector4 Vector4::operator / ( float scalar ) const
	{
		return Vector4( x / scalar, y / scalar, z / scalar, w / scalar );
	}

	Vector4 Vector4::operator / ( const Vector4& vector ) const
	{
		return Vector4( x / vector.x, y / vector.y, z / vector.z, w / vector.w );
	}

	/** @} */
	/** @} */
}