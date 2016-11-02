#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/


	/** 
		自动伸长缓冲
		@remark
			按照固定步长自动扩充的连续存储缓冲
		@par
			避免使用vector/array等的不透明实现造成的结构，算法复杂度提高
		@param
			TType 元素类型
		@param
			ALLOC_STEP 分配步长
	*/
	template <class TType, dword ALLOC_STEP >
	class growable_buffer
	{
	public:
		TType*	mTypeData;
		dword	mAllocedSize;
		dword	mDataSize;

		growable_buffer( )
			: mTypeData( null )
			, mAllocedSize( 0 )
			, mDataSize( 0 )
		{
		
		}

		~growable_buffer( )
		{
			clear();
		}

		/** 
			获取缓冲指针
			@return
				获取缓冲地址
		*/
		void* getdata( ) const
		{
			return mTypeData;
		}

		/** 
			获取指定索引的元素引用
			@param
				index 索引
			@return
				元素引用
		*/
		TType& operator [] ( dword index )
		{
			return *at( index );
		}

		/** 
			获取指定索引的元素指针
			@param
				index 索引
			@return
				元素指针，若元素不存在，返回null
		*/
		TType* at( dword index )
		{
			// 在范围内,取原数据
			if ( index < mAllocedSize )
				return mTypeData + index;

			// 超过, 找到一个合适步长
			dword finalsize = mAllocedSize;
			while ( index >= finalsize )
			{
				finalsize += ALLOC_STEP;
			}
			
			// 重新分配内存
			resize( finalsize );

			return mTypeData + index;
		}

		/** 
			推入一个新元素
			@return
				返回刚推入元素的指针
		*/
		TType* push_advance( )
		{
			return at( mDataSize++ );
		}

		/** 
			推入一个新元素
			@param
				data 将一个新元素拷贝到新推入的内存中
		*/
		void push_advance( const TType& data )
		{
			*at( mDataSize++ ) = data;
		}

		/** 
			获得缓冲大小
			@return
				获得缓冲大小
		*/
		dword size( ) const
		{
			return mDataSize;
		}

		/** 
			重置缓冲大小
						
		*/
		void reset( )
		{
			mDataSize = 0;
		}

		/** 
			清除内存
			@note
				本函数将会释放内存，造成性能问题
		*/
		void clear( )
		{
			for ( dword i = 0;i< mAllocedSize;i++)
			{
				type_destructor( mTypeData + i );
			}

			free( mTypeData );
			mTypeData = null;
			mAllocedSize = 0;
			mDataSize = 0;
		}

	private:
		void resize( dword newsize )
		{
			for ( dword i = 0;i< mAllocedSize;i++)
			{
				type_destructor( mTypeData + i );
			}

			mTypeData = (TType*)realloc( mTypeData, sizeof(TType) * newsize );
			mAllocedSize = newsize;

			for ( dword i = 0;i< mAllocedSize;i++)
			{
				type_constructor( mTypeData + i );
			}
		}
	};

	/** @} */
	/** @} */
}