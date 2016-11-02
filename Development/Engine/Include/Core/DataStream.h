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
		内存定位方向
	*/
	enum eMemoerySeekOrigin
	{
		MSO_Begin,			///< 从流开始定位
		MSO_Current,		///< 从当前指针位置定位
		MSO_End,			///< 从流尾开始定位
	};

	/** 
		内存抽象流
		@remark
			本流需要由其他流实现
	*/
	class DataStream
	{
	public:
		DataStream( );

		virtual ~DataStream( );

		/** 
			写缓冲
			@param
				input 输入数据
			@param
				numberOfBytes 写入字节数
			@return
				是否成功写入
		*/
		virtual bool	WriteBuffer( const void* input, dword numberOfBytes );

		/** 
			读缓冲
			@param
				output 接收缓冲
			@param
				numberOfBytes 读入字节数
			@return
				是否成功读取,字节数不够时视为读取失败
		*/
		virtual bool	ReadBuffer( void* output, dword numberOfBytesToRead );

		/** 
			读缓冲
			@param
				s 源数据内存流
			@param
				numberOfBytes 读入字节数
			@param
				extendbufferalloc 多余的分配字节数
			@return
				是否成功读取,字节数不够时视为读取失败
		*/
		bool			ReadBuffer( DataStream* s, dword numberOfBytes , dword extendbufferalloc = 0);

		/** 
			写缓冲
			@param
				s 源数据内存流
			@return
				是否成功读取,字节数不够时视为读取失败
		*/
		bool			WriteBuffer( DataStream* s );

		/** 
			查看流还有多少字节可以读取
			@return
				字节数
			@remark
				仅在读取时可用
		*/
		virtual dword	GetRemainBytes( )	const; 

		/** 
			查看流指针位置
		*/
		virtual dword	GetPosition()	const;	

		/** 
			定位指针
			@param
				offset 文件偏移,可正负
			@param
				mso 搜索方向
		*/
		virtual bool	Seek( int offset, eMemoerySeekOrigin mso );		// 读方式搜索

		/** 
			忽略字节数
			@remark
				仅在读方式下可用
		*/
		virtual bool	IgnoreBytes( dword numberOfBytes );

		/** 
			设置流长度
			@param
				len 长度
			@remark
				仅在写模式可用

		*/
		virtual void	SetLength( dword len );

		/** 
			复位流指针
		*/
		virtual void	Reset( );

		/** 
			取得缓冲地址
		*/
		virtual char*	GetBuffer( ) = 0;

		/** 
			取得流大小
		*/
		virtual dword	GetSize( ) const;

		/** 
			分配内部存储空间
		*/
		virtual char*	AllocBuffer( dword size );

		/** 
			释放内部存储空间
		*/
		virtual void	ReleaseBuffer( );

		/** 
			流是否可用
		*/
		virtual bool	IsValid( );

		/// 模板写数据
		template<typename T>
		bool	WriteTypes( const T& v )		{	return WriteBuffer( &v, sizeof(T) );}

		/// 写入Ansi字符串
		bool	WriteTypes( const AString& v )  { return WriteANSIString( v.c_str()); }

		/// 写入Unicode字符串
		bool	WriteTypes( const WString& v )  { return WriteUNICODEString( v.c_str()); }

		/// 写入Ansi字符串
		bool	WriteTypes( const char* v )  { return WriteANSIString( v ); }

		/// 写入Unicode字符串
		bool	WriteTypes( const wchar* v )  { return WriteUNICODEString( v ); }


		/// 模板读数据
		template<typename T>
		bool	ReadTypes( T& Ret )		{	return ReadBuffer( &Ret, sizeof(T)  );}

		/// 读入Ansi字符串
		template<> 
		bool ReadTypes( AString& Ret )				{ return ReadString<AString, char>( Ret ); }

		/// 读入Unicode字符串
		template<>
		bool ReadTypes( WString& Ret)				{ return ReadString<WString, wchar>( Ret ); }

		/// 读入Ansi字符串,忽略错误,默认值为空
		AString ReadANSIString( );

		/// 读入Unicode字符串,忽略错误,默认值为空
		WString ReadUNICODEString( );

		/** 
			忽略Unicode字符串
			@remark
				仅在读模式可用
		*/
		bool	IgnoreUNICODEString( );

		/** 
			忽略Ansi字符串
			@remark
				仅在读模式可用
		*/
		bool	IgnoreANSIString( );

		/// 写入Unicode字符串
		bool	WriteUNICODEString( const WString& str );

		/// 写入Unicode字符串
		bool	WriteUNICODEString( const wchar* str );

		/// 写入Ansi字符串
		bool	WriteANSIString( const AString& str );

		/// 写入Ansi字符串
		bool	WriteANSIString( const char* str );

	private:
		template<typename T, typename T2>
		bool		ReadString( T& Str );		
		template<typename T> 
		bool	WriteString( const T* str, dword len );

	protected:
		dword		mDataSize;		// 数据实际大小
		dword		mPointer;		// 读 位置
		dword		mAllocedSize;	// 已经分配的内存大小
	};

	/** @} */
	/** @} */
}