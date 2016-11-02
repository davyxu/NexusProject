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
		引用数据流
	*/
	class RefStream : public DataStream
	{
	public:
		RefStream( );

		/** 
			构造需要引用数据入口
			@param
				data 需要引用的数据
			@param
				size 需要引用的数据大小
		*/
		RefStream( const void* data, dword size	);

		/** 
			构造需要引用数据的流
			@param
				m 需要引用的流
		*/
		RefStream( DataStream* m );

		/** 
			构造需要引用数据的流
			@param
				m 需要引用的流
			@param
				numberOfBytes 指定size
		*/
		RefStream( DataStream* m, dword numberOfBytes );

		virtual bool	WriteBuffer( const void* input, dword numberOfBytes );

		virtual char*	GetBuffer( );

		virtual char*	AllocBuffer( dword size );

		/// 二进制序列化操作符重载
		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, RefStream& Data )
		{
			DataStream* Stream = Ser.GetStream();

			Assert( Ser.IsSaving() )		
			Stream->WriteTypes<dword>( Data.GetSize() );
			Stream->WriteBuffer( &Data );			

			return Ser;
		}

	protected:
		const char* mRefData;	// 引用方式读取缓冲
	};

	/** @} */
	/** @} */
}