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
		点封装类
	*/
	class Point
	{
	public:

		union
		{
			struct 
			{
				int x, y;
			};

			struct 
			{
				int width,height;
			};
		};

		/// 零点
		const static Point cZero;

		/// 整形最大点0x7FFFFFFF, 0x7FFFFFFF
		const static Point cMaxPoint;

		/// 整形最小点0x80000001, 0x80000001
		const static Point cMinPoint;

		/// 构造
		inline Point( );

		/// 赋值构造
		inline Point( int xx, int yy );


		inline Point operator + ( ) const;

		/// 取负
		inline Point operator - ( ) const;

		/// 判定相等
		inline bool operator == ( const Point& point ) const;

		/// 判定不等
		inline bool operator != ( const Point& point ) const;

		/// 自加
		inline Point& operator += ( const Point& point );

		/// 自减
		inline Point& operator -= ( const Point& point );

		/// 加
		inline Point operator + ( const Point& point ) const;

		/// 减
		inline Point operator - ( const Point& point ) const;

		/// 乘
		inline Point Point::operator * ( int scale ) const;

		/// 除
		inline Point Point::operator / ( int scale ) const;

		/// 计算两点距离
		static int DistanceTo(const Point& v1, const Point& v2);

		/// 计算亮点距离平方
		static int DistanceToSqare(const Point& v1, const Point& v2);
	};

	//----------------------------------------------------------------------------
	// Point Implementation
	//----------------------------------------------------------------------------

	Point::Point( )
	{
	}

	Point::Point( int xx, int yy ) : x( xx ), y( yy )
	{
	}

	Point Point::operator + ( ) const
	{
		return Point( x, y ); 
	}

	Point Point::operator - ( ) const
	{
		return Point( -x, -y );
	}

	bool Point::operator == ( const Point& point ) const
	{
		return x == point.x && y == point.y;
	}

	bool Point::operator != ( const Point& point ) const
	{
		return x != point.x || y != point.y;
	}

	Point& Point::operator += ( const Point& point )
	{
		x += point.x;
		y += point.y;

		return *this;
	}

	Point& Point::operator -= ( const Point& point )
	{
		x -= point.x;
		y -= point.y;

		return *this;
	}

	Point Point::operator + ( const Point& point ) const
	{
		return Point( x + point.x, y + point.y );
	}

	Point Point::operator - ( const Point& point ) const
	{
		return Point( x - point.x, y - point.y );
	}


	Point Point::operator * ( int scale ) const
	{
		return Point( x * scale, y * scale );
	}

	Point Point::operator / ( int scale ) const
	{
		return Point( x / scale, y / scale );
	}

	/** @} */
	/** @} */
};

