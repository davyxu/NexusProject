#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/

	#pragma warning(disable:4201)
	class Matrix3;

	/** 
		矢量2
	*/
	class Vector2
	{
	public:

		float x, y;

		/// 零
		const static Vector2 cZero;

		/// 一
		const static Vector2 cOne;

		/// x=1
		const static Vector2 cXAxis;

		/// y=1
		const static Vector2 cYAxis;

		/// 默认构造
		inline Vector2( );

		/// 2值构造
		inline Vector2( float xx, float yy );

		/// 正
		inline Vector2 operator + ( ) const;
		
		/// 负
		inline Vector2 operator - ( ) const;

		/// 自加
		inline Vector2& operator += ( const Vector2& vector );
		
		/// 自减
		inline Vector2& operator -= ( const Vector2& vector );
		
		/// 乘法因子
		inline Vector2& operator *= ( float scalar );

		/// 自乘
		inline Vector2& operator *= ( const Vector2& vector );

		Vector2& operator *= ( const Matrix3& matrix );

		/// 除法因子
		inline Vector2& operator /= ( float scalar );
		
		/// 自除
		inline Vector2& operator /= ( const Vector2& vector );
		
		/// 加
		inline Vector2 operator + ( const Vector2& vector ) const;
		
		/// 减
		inline Vector2 operator - ( const Vector2& vector ) const;
		
		/// 乘因子
		inline Vector2 operator * ( float scalar ) const;
		
		/// 乘
		inline Vector2 operator * ( const Vector2& vector ) const;

		Vector2 operator * ( const Matrix3& matrix ) const;

		/// 除因子
		inline Vector2 operator / ( float scalar ) const;
		
		/// 除
		inline Vector2 operator / ( const Vector2& vector ) const;

		/// 等于判断
		bool operator == (const Vector2 &v) const;

		/// 不等判断
		bool operator != (const Vector2 &v) const;

		bool Equal( const Vector2& vector, float margin = Math::cEpsilon ) const;

		/// 单位化
		Vector2& Normalize( );

		/// 模
		float Magnitude( ) const;
		
		/// 平方模
		float MagnitudeSquared( ) const;

		/// 点乘
		static float DotProduct( const Vector2& vector1, const Vector2& vector2 );
		
		/// 叉乘
		static float CrossProduct( const Vector2& vector1, const Vector2& vector2 );
		
		/// 投射
		static Vector2 Project( const Vector2& vector1, const Vector2& vector2 );
		
		/// 线性插值
		static Vector2 Lerp( const Vector2& vector1, const Vector2& vector2, float factor );

		/// 计算两点间距离
		static float DistanceTo(const Vector2& v1, const Vector2& v2);

		/// 计算两点间距离平方
		static float DistanceToSqare(const Vector2& v1, const Vector2& v2);

		/// 2D变换
		Vector2& Mul2D( const Matrix3& matrix );
	};



	Vector2::Vector2( )
	{
	}

	Vector2::Vector2( float xx, float yy ) : x( xx ), y( yy )
	{
	}

	Vector2 Vector2::operator + ( ) const
	{
		return Vector2( x, y ); 
	}

	Vector2 Vector2::operator - ( ) const
	{
		return Vector2( -x, -y );
	}

	Vector2& Vector2::operator += ( const Vector2& vector )
	{
		x += vector.x;
		y += vector.y;

		return *this;
	}

	Vector2& Vector2::operator -= ( const Vector2& vector )
	{
		x -= vector.x;
		y -= vector.y;

		return *this;
	}

	Vector2& Vector2::operator *= ( float scalar )
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vector2& Vector2::operator *= ( const Vector2& vector )
	{
		x *= vector.x;
		y *= vector.y;

		return *this;
	}

	Vector2& Vector2::operator /= ( float scalar )
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	Vector2& Vector2::operator /= ( const Vector2& vector )
	{
		x /= vector.x;
		y /= vector.y;

		return *this;
	}

	Vector2 Vector2::operator + ( const Vector2& vector ) const
	{
		return Vector2( x + vector.x, y + vector.y );
	}

	Vector2 Vector2::operator - ( const Vector2& vector ) const
	{
		return Vector2( x - vector.x, y - vector.y ); 
	}

	Vector2 Vector2::operator * ( float scalar ) const
	{
		return Vector2( x * scalar, y * scalar );
	}

	Vector2 Vector2::operator * ( const Vector2& vector ) const
	{
		return Vector2( x * vector.x, y * vector.y );
	}

	Vector2 Vector2::operator / ( float scalar ) const
	{
		return Vector2( x / scalar, y / scalar );
	}

	Vector2 Vector2::operator / ( const Vector2& vector ) const
	{
		return Vector2( x / vector.x, y / vector.y );
	}

	/** @} */
	/** @} */
}
