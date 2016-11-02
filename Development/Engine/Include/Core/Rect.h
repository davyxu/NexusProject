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
		矩形类
	*/
	class Rect
	{
	public:
		int	left,top,right,bottom;

		/** 
			空矩形，所有参数为0
		*/
		const static Rect cNullRect;

		/** 
			构造函数
		*/
		inline Rect( );

		/** 
			构造函数
			@remark
				使用4分量初始化
		*/
		inline Rect( int ll, int tt, int rr, int bb );

		/** 
			构造函数
			@remark
				使用左上角，右下角初始化
		*/
		inline Rect( const Point& leftTop,const Point& rightBottom );

		inline Rect operator + ( ) const;

		/** 
			取负
			@return
				所有分量取负后的对象
		*/
		inline Rect operator - ( ) const;

		/** 
			判断两个对象是否相等
		*/
		inline bool operator == ( const Rect& rect );

		/** 
			判断两个对象是否不等
		*/
		inline bool operator != ( const Rect& rect );

		/** 
			自加
			@param
				point 增量
		*/
		inline Rect& operator += ( const Point& point );

		/** 
			自减
			@param
				point 增量
		*/
		inline Rect& operator -= ( const Point& point );

		/** 
			加操作
		*/
		inline Rect operator + ( const Point& point ) const;
		
		/** 
			减操作
		*/
		inline Rect operator - ( const Point& point ) const;

		/// 取宽度
		inline int Width( ) const;

		/// 取高度
		inline int Height( ) const;

		/// 取中心
		inline Point Center( ) const;

		/// 取大小
		inline Point GetSize( ) const;

		/// 取左上
		inline Point LeftTop( ) const;
		
		/// 取左下
		inline Point LeftBottom( ) const;
		
		/// 取右上
		inline Point RightTop( ) const;
		
		/// 取右下
		inline Point RightBottom( ) const;

		/** 
			偏移操作
			@param
				x 分量偏移
			@param
				y 分量偏移
		*/
		inline void Offset( int x,int y);


		inline void Offset( const Point& pt );

		/** 
			膨胀,相对中心	
			@param
				width 分量
			@param
				height 分量
		*/
		inline void Inflate( int width, int height );
		
		/** 
			膨胀,相对中心	
			@param
				size 分量
		*/
		inline void Inflate( const Point& size );
		
		/** 
			收缩,相对中心	
			@param
				width 分量
			@param
				height 分量
		*/
		inline void Deflate( int width, int height );

		/** 
			收缩,相对中心	
			@param
				size 分量
		*/
		inline void Deflate( const Point& size );

		/// 判断一个点是否在矩形区域内
		inline bool PointInRect( const Point& point ) const;

		/** 
			取矩形区域交集的区域
			@param
				rect1 矩形1
			@param
				rect2 矩形2
			@return
				取交后的矩形
		*/
		static Rect Intersect( const Rect& rect1, const Rect& rect2 );

		/** 
			合并两个矩形区域
			@param
				rect1 矩形1
			@param
				rect2 矩形2
			@return
				合并后的矩形
		*/
		static Rect Unite( const Rect& rect1, const Rect& rect2 );

		/// 剪裁源区域
		static Rect Clip( const Rect& source, const Rect& cliprc );
	};

	//----------------------------------------------------------------------------
	// Rect Implementation
	//----------------------------------------------------------------------------

	Rect::Rect( )
	{
	}

	Rect::Rect( int ll, int tt, int rr, int bb ) : left( ll ), top( tt ), right( rr ), bottom( bb )
	{
	}

	Rect::Rect( const Point& leftTop,const Point& rightBottom ):
	left(leftTop.x),top(leftTop.y),
	right(rightBottom.x),bottom(rightBottom.y)
	{

	}

	Rect Rect::operator + ( ) const
	{
		return Rect( left, top, right, bottom );
	}

	Rect Rect::operator - ( ) const
	{
		return Rect( -left, -top, -right, -bottom );
	}

	bool Rect::operator == ( const Rect& rect )
	{
		return left == rect.left && top == rect.top && right == rect.right && bottom == rect.bottom;
	}

	bool Rect::operator != ( const Rect& rect )
	{
		return left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom;
	}


	Rect& Rect::operator += ( const Point& point )
	{
		left += point.x;
		top += point.y;
		right += point.x;
		bottom += point.y;

		return *this;
	}

	Rect& Rect::operator -= ( const Point& point )
	{
		left -= point.x;
		top -= point.y;
		right -= point.x;
		bottom -= point.y;

		return *this;
	}

	Rect Rect::operator + ( const Point& point ) const
	{
		return Rect( left + point.x, top + point.y, right + point.x, bottom + point.y );
	}

	Rect Rect::operator - ( const Point& point ) const
	{
		return Rect( left - point.x, top - point.y, right - point.x, bottom - point.y );
	}

	int Rect::Width( ) const
	{
		return right - left;
	}

	int Rect::Height( ) const
	{
		return bottom - top;
	}

	Point Rect::Center( ) const
	{
		return Point( ( left + right ) / 2, ( top + bottom ) / 2 );
	}

	Point Rect::GetSize( ) const
	{
		return Point(right - left,bottom - top);
	}

	Point Rect::LeftTop( ) const
	{
		return Point( left, top );
	}

	Point Rect::LeftBottom( ) const
	{
		return Point( left, bottom );
	}

	Point Rect::RightTop( ) const
	{
		return Point( right, top );
	}

	Point Rect::RightBottom( ) const
	{
		return Point( right, bottom );
	}

	void Rect::Inflate( int width, int height )
	{
		left -= width;
		right += width;
		top -= height;
		bottom += height;
	}

	void Rect::Inflate( const Point& size )
	{
		left -= size.x;
		right += size.x;
		top -= size.y;
		bottom += size.y;
	}

	void Rect::Deflate( int width, int height )
	{
		left += width;
		right -= width;
		top += height;
		bottom -= height;
	}

	void Rect::Deflate( const Point& size )
	{
		left += size.x;
		right -= size.x;
		top += size.y;
		bottom -= size.y;
	}

	void Rect::Offset( int x,int y)
	{
		left += x;
		top += y;
		right += x;
		bottom += y;
	}

	void Rect::Offset(  const Point& pt )
	{
		left += pt.x;
		top += pt.y;
		right += pt.x;
		bottom += pt.y;
	}
	

	bool Rect::PointInRect( const Point& point ) const
	{
		return point.x >= left && point.x <= right && point.y >= top && point.y <= bottom;
	}

	/** @} */
	/** @} */
}
