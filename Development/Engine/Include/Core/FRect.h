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
		浮点矩形类
	*/
	class FRect
	{
	public:
		float	left,top,right,bottom;

		/** 
			空矩形，所有参数为0
		*/
		const static FRect cNullRect;

		/** 
			构造函数
		*/
		inline FRect( );

		/** 
			构造函数
			@remark
				使用4分量初始化
		*/
		inline FRect( float ll, float tt, float rr, float bb );

		/** 
			构造函数
			@remark
				使用左上角，右下角初始化
		*/
		inline FRect( const Vector2& leftTop,const Vector2& rightBottom );

		/** 
			取负
			@return
				所有分量取负后的对象
		*/
		inline FRect operator - ( ) const;

		/** 
			判断两个对象是否相等
		*/
		inline bool operator == ( const FRect& rect );

		/** 
			判断两个对象是否不等
		*/
		inline bool operator != ( const FRect& rect );

		/** 
			自加
			@param
				point 增量
		*/
		inline FRect& operator += ( const Vector2& point );

		/** 
			自减
			@param
				point 增量
		*/
		inline FRect& operator -= ( const Vector2& point );

		/** 
			加操作
		*/
		inline FRect operator + ( const Vector2& point ) const;

		/** 
			减操作
		*/
		inline FRect operator - ( const Vector2& point ) const;

		
		/// 取宽度
		inline float Width( ) const;

		/// 取高度
		inline float Height( ) const;

		/// 取中心
		inline Vector2 Center( ) const;

		/// 取大小
		inline Vector2 GetSize( ) const;

		/// 取左上
		inline Vector2 LeftTop( ) const;

		/// 取左下
		inline Vector2 LeftBottom( ) const;

		/// 取右上
		inline Vector2 RightTop( ) const;

		/// 取右下
		inline Vector2 RightBottom( ) const;

		/** 
			偏移操作
			@param
				x 分量偏移
			@param
				y 分量偏移
		*/
		inline void Offset( float x,float y);

		/** 
			膨胀,相对中心	
			@param
				width 分量
			@param
				height 分量
		*/
		inline void Inflate( float width, float height );

		/** 
			膨胀,相对中心	
			@param
				size 分量
		*/
		inline void Inflate( const Vector2& size );

		/** 
			收缩,相对中心	
			@param
				width 分量
			@param
				height 分量
		*/
		inline void Deflate( float width, float height );

		/** 
			收缩,相对中心	
			@param
				size 分量
		*/
		inline void Deflate( const Vector2& size );

		/// 判断一个点是否在矩形区域内
		inline bool PointInRect( const Vector2& point ) const;

		/** 
			取矩形区域交集的区域
			@param
				rect1 矩形1
			@param
				rect2 矩形2
			@return
				取交后的矩形
		*/
		static FRect Intersect( const FRect& rect1, const FRect& rect2 );

		/** 
			合并两个矩形区域
			@param
				rect1 矩形1
			@param
				rect2 矩形2
			@return
				合并后的矩形
		*/
		static FRect Unite( const FRect& rect1, const FRect& rect2 );
	};

	//----------------------------------------------------------------------------
	// FRect Implementation
	//----------------------------------------------------------------------------

	FRect::FRect( )
	{
	}

	FRect::FRect( float ll, float tt, float rr, float bb ) : left( ll ), top( tt ), right( rr ), bottom( bb )
	{
	}

	FRect::FRect( const Vector2& leftTop,const Vector2& rightBottom ):
	left(leftTop.x),top(leftTop.y),
		right(rightBottom.x),bottom(rightBottom.y)
	{

	}

	FRect FRect::operator - ( ) const
	{
		return FRect( -left, -top, -right, -bottom );
	}

	bool FRect::operator == ( const FRect& rect )
	{
		return left == rect.left && top == rect.top && right == rect.right && bottom == rect.bottom;
	}

	bool FRect::operator != ( const FRect& rect )
	{
		return left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom;
	}


	FRect& FRect::operator += ( const Vector2& point )
	{
		left += point.x;
		top += point.y;
		right += point.x;
		bottom += point.y;

		return *this;
	}

	FRect& FRect::operator -= ( const Vector2& point )
	{
		left -= point.x;
		top -= point.y;
		right -= point.x;
		bottom -= point.y;

		return *this;
	}

	FRect FRect::operator + ( const Vector2& point ) const
	{
		return FRect( left + point.x, top + point.y, right + point.x, bottom + point.y );
	}

	FRect FRect::operator - ( const Vector2& point ) const
	{
		return FRect( left - point.x, top - point.y, right - point.x, bottom - point.y );
	}

	float FRect::Width( ) const
	{
		return right - left;
	}

	float FRect::Height( ) const
	{
		return bottom - top;
	}

	Vector2 FRect::Center( ) const
	{
		return Vector2( ( left + right ) / 2, ( top + bottom ) / 2 );
	}

	Vector2 FRect::GetSize( ) const
	{
		return Vector2(right - left,bottom - top);
	}

	Vector2 FRect::LeftTop( ) const
	{
		return Vector2( left, top );
	}

	Vector2 FRect::LeftBottom( ) const
	{
		return Vector2( left, bottom );
	}

	Vector2 FRect::RightTop( ) const
	{
		return Vector2( right, top );
	}

	Vector2 FRect::RightBottom( ) const
	{
		return Vector2( right, bottom );
	}

	void FRect::Inflate( float width, float height )
	{
		left -= width;
		right += width;
		top -= height;
		bottom += height;
	}

	void FRect::Inflate( const Vector2& size )
	{
		left -= size.x;
		right += size.x;
		top -= size.y;
		bottom += size.y;
	}

	void FRect::Deflate( float width, float height )
	{
		left += width;
		right -= width;
		top += height;
		bottom -= height;
	}

	void FRect::Deflate( const Vector2& size )
	{
		left += size.x;
		right -= size.x;
		top += size.y;
		bottom -= size.y;
	}

	void FRect::Offset( float x,float y)
	{
		left += x;
		top += y;
		right += x;
		bottom += y;
	}

	bool FRect::PointInRect( const Vector2& point ) const
	{
		return point.x >= left && point.x <= right && point.y >= top && point.y <= bottom;
	}
}
