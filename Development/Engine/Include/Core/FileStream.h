#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Stream
	*  @{
	*/


	/// 文件访问模式
	enum eFileAccessMode
	{ 
		FAM_Invalid,		///< 非法访问模式
		FAM_Read,			///< 读方式
		FAM_Write,			///< 写方式
	};

	/** 
		文件流
	*/
	class  FileStream : public DataStream
	{
	public:
		FileStream( );

		virtual ~FileStream( );

		/** 
			构造时打开文件
			@param
				filename 打开的文件名
			@param
				accessmode 访问模式
			@param
				openalways 始终打开方式
			@remark
				注意使用Vaild函数判断文件是否成功打开
		*/
		FileStream( const wchar* filename, eFileAccessMode accessmode, bool openalways = false );

		/** 
			将缓冲写入到文件
			@param
				input 输入数据
			@param
				numberOfBytes 写入字节数
			@return
				是否成功写入
		*/
		virtual bool	WriteBuffer( const void* input, dword numberOfBytes );

		/** 
			写缓冲
			@param
				s 源数据内存流
			@return
				是否成功读取,字节数不够时视为读取失败
		*/
		bool			WriteBuffer( DataStream* s );


		/** 
			从文件读入缓冲
			@param
				output 接收缓冲
			@param
				numberOfBytes 读入字节数
			@return
				是否成功读取,字节数不够时视为读取失败
		*/
		virtual bool	ReadBuffer( void* output, dword numberOfBytesToRead );

		/** 
			查看文件还有多少字节可以读取
			@return
				字节数
			@remark
				仅在读取时可用
		*/
		virtual dword	GetRemainBytes( ) const;

		/** 
			查看文件指针位置
		*/
		virtual dword	GetPosition() const;

		/** 
			定位指针
			@param
				offset 文件偏移,可正负
			@param
				mso 搜索方向
		*/
		virtual bool	Seek( int offset, eMemoerySeekOrigin mso );

		/** 
			设置文件长度
			@param
				len 文件长度
			@remark
				仅在写模式可用

		*/
		virtual void	SetLength( dword len );

		/** 
			复位文件指针
		*/
		virtual void	Reset( );

		/** 
			FileStream中不可分配缓冲
		*/
		virtual char*	AllocBuffer( dword size );

		/** 
			FileStream不可获得缓冲地址
		*/
		virtual char*	GetBuffer( );

		/** 
			取得文件大小
		*/
		virtual dword	GetSize( ) const;

		/** 
			手动打开文件
			@param
				filename 打开的文件名
			@param
				accessmode 访问模式
			@param
				openalways 始终打开方式
			@return
				打开是否成功
		*/
		bool			Open( const wchar* filename, eFileAccessMode accessmode, bool openalways = false );

		/** 
			关闭文件
		*/
		void			Close( );

		/** 
			取文件句柄
		*/
		handle			GetHandle( ){ return mFile;}

		/** 
			文件是否可以正常操作
		*/
		virtual bool	IsValid( );

		/** 
			取得当前文件操作模式
		*/
		eFileAccessMode GetMode( ){ return mFileAccessMode; }
		
	protected:
		handle	mFile;
		eFileAccessMode mFileAccessMode;
	};

	/** @} */
	/** @} */
}