#pragma once
 
namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Stream
	*  @{
	*/


	/** 
		堆内存流
	*/
	class HeapStream : public DataStream
	{
	public:
		enum{
			ALIGN_ALLOC_STEP = 1024, ///< 默认分配步长
		};

		/** 
			构造函数
			@param
				prealloc 预分配大小
		*/
		HeapStream( dword prealloc = 0 );

		/** 
			释放内存
		*/
		virtual ~HeapStream( );

		/** 
			将数据写入到流
			@param
				data 数据
			@param
				size 写入的数据大小
		*/
		HeapStream( const void* data, dword size );

		/** 
			将数据写入到流
			@param
				m 内存流
			@param
				extendbuffersize 额外分配的内存大小
			@remark
				使用Heap缓冲FileStream并作为ANSI字符串流时，应该设置buffersize+1
		*/
		HeapStream( DataStream* m, dword extendbuffersize = 0 );

		/** 
			取得缓冲地址
		*/
		virtual char*	GetBuffer( );

		/** 
			分配缓冲
			@param
				size 要分配大小
			@return
				返回分配好的大小
		*/
		virtual char*	AllocBuffer( dword size );

		/** 
			释放分配的缓冲
		*/
		virtual void	ReleaseBuffer( );

		/// 二进制序列号操作符重载
		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, HeapStream*& Data )
		{
			DataStream* Stream = Ser.GetStream();
			
			if ( Ser.IsLoading() )
			{
				Data = new HeapStream;
				dword BufferSize = 0;
				Stream->ReadTypes<dword>( BufferSize );

				if ( BufferSize > 0 )
					Stream->ReadBuffer( Data, BufferSize );
			}
			else
			{
				Stream->WriteTypes<dword>( Data->GetSize() );
				Stream->WriteBuffer( Data );
			}
				
			return Ser;
		}

		/// 二进制序列号操作符重载
		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, HeapStream& Data )
		{
			DataStream* Stream = Ser.GetStream();

			if ( Ser.IsLoading() )
			{
				dword BufferSize = 0;
				Stream->ReadTypes( BufferSize );

				if ( BufferSize > 0 )
					Stream->ReadBuffer( &Data, BufferSize );
			}
			else
			{
				Stream->WriteTypes<dword>( Data.GetSize() );
				Stream->WriteBuffer( &Data );
			}

			return Ser;
		}

	protected:
		char* mHeapData;		// 动态大包数据
	};

	/** @} */
	/** @} */
}