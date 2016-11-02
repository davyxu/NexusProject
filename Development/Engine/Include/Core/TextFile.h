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
		文本文件读写类
	*/
	class TextFile
	{
	public:
		TextFile( );
		virtual ~TextFile( );

		/** 
			打开一个文本文件
		*/
		bool	Open( const wchar* filename );

		/** 
			创建一个文本文件
			@param
				filename 文件名
			@param
				UnicodeFormat 是否是Unicode模式
			@return
				是否创建成功
		*/
		bool	Create( const wchar* filename, bool UnicodeFormat = true );

		/** 
			从一个流打开一个文本文件
		*/
		bool	Open( DataStream* stream, bool copymemory = false );
		
		/** 
			创建一个文本文件
			@param
				stream 来源流
			@param
				UnicodeFormat 是否是Unicode模式
			@return
				是否创建成功
		*/
		bool	Create(  DataStream* stream, bool UnicodeFormat = true);
		
		/** 
			文件是否为Unicode格式
		*/
		bool	IsUnicode( void );

		/** 
			格式化写入一行文本
		*/
		bool	WriteLine(const wchar* format, ... );

		/** 
			读取一行文本
		*/
		bool	ReadLine( WString& str );

		/** 
			文本是否结束
		*/
		bool	IsEnd( );

		/** 
			关闭文件
		*/
		void Close( );
	private:
		bool			mIsUnicode;		// unicode标识
		WString			mDivision;		// Trim时切分符
		DataStream*	mStream;
		FileStream*		mFile;
		StringConverter mConv;
		bool			mDeleteStream;
	};

	/** @} */
	/** @} */
}
