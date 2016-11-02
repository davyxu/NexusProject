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
		字符串格式化，常用操作类
	*/
	class StringHelperA
	{
	public:
		/** 
			格式化字符串
			@param
				format 格式化参数或者字符串
			@return
				格式化后的字符串
		*/
		static AString FormatString( const char* format, ... );

		/** 
			格式化字符串，使用栈
			@param
				format 格式化参数或者字符串
			@param
				arguments 参数指针，一般参数为可变参数时，使用(char*)( &format ) + sizeof( format )
			@return
				格式化后的字符串
		*/
		static AString FormatStringV( const char* format, const void* arguments );

		/** 
			格式化字符串缓冲
			@param
				buffer 结果输出缓冲
			@param
				size 结果输出缓冲大小
			@param
				format 格式化参数或者字符串
			@return
				格式化后的字符串
		*/
		static const char* FormatBuffer( char* buffer, size_t size, const char* format, ... );

		/** 
			格式化字符串缓冲，使用栈
			@param
				buffer 结果输出缓冲
			@param
				size 结果输出缓冲大小
			@param
				format 格式化参数或者字符串
			@param
				arguments 参数指针，一般参数为可变参数时，使用(char*)( &format ) + sizeof( format )
			@return
				格式化后的字符串
		*/
		static const char* FormatBufferV( char* buffer, size_t size, const char* format, const void* arguments );

		/** 
			字符串连接
			@remark
				连接字符串1，2， 将结果放置在1当中
			@param
				buffer 字符串1，结果放置缓冲
			@param
				source 字符串2
		*/
		static void Connect(char* buffer, const char* source);

		/** 
			字符串比较
			@param
				str1 字符串1
			@param
				str2 字符串2
			@return
				字符串是否相等
		*/
		static bool Compare( const char* str1, const char* str2 );

		/** 
			字符串拷贝
			@remark
				将源字符串内容拷贝到目标缓冲中
			@param
				buffer 目标字符串缓冲
			@param
				source 源字符串
			@param
				size 拷贝的字符串数量，-1表示拷贝整个字符串
		*/
		static void Copy(char* buffer,const char* source,int size = -1 );

		/** 
			字符串忽略大小写比较
			@param
				str1 字符串1
			@param
				str2 字符串2
			@return
				字符串是否相等
		*/
		static bool CompareLowerCase( const char* str1, const char* str2 );

		/** 
			获得字符串长度
			@param
				buffer 字符串
			@return
				字符串长度
		*/
		static dword Length( const char* buffer );

		/** 
			获取字符串右部分
			@param
				str 输入字符串
			@param
				count 截取数目
			@return
				截取的字符串
		*/
		static AString		Right(const AString& str, size_t count);

		/** 
			获取字符串左部分
			@param
				str 输入字符串
			@param
				count 截取数目
			@return
				截取的字符串
		*/
		static AString		Left(const AString& str, size_t count);

		/** 
			获取字符串中间部分
			@param
				str 输入字符串
			@param
				count 截取数目
			@return
				截取的字符串
		*/
		static AString		Mid( const AString& str, size_t begin,size_t end);

		/** 
			获取字符串占用内存数
			@param
				str 输入字符串
			@return
				字节数
		*/
		static dword		SizeOfBytes( const char* str );

		/** 
			去除字符串中左边多余的字符串
			@param
				str 输入字符串
			@param
				delims 去除的部分
			@return
				去除后的字符串
		*/
		static AString		TrimLeft( const char* str ,const char* delims = " \t\r" );

		/** 
			去除字符串中右边多余的字符串
			@param
				str 输入字符串
			@param
				delims 去除的部分
			@return
				去除后的字符串
		*/
		static AString		TrimRight( const char* str ,const char* delims = " \t\r" );

		/** 
			格式化解析字符串
			@remark
				需要将需要获取的变量以指针方式放置在可选参数中
			@param
				str 输入字符串
			@param
				format 解析格式
			@return
				本次解析识别的变量数
		*/
		static int			Parse( const char* str, const char* format, ... );

		/** 
			将字符串中字母部分大写变小写
			@param
				str 输入字符串
			@return
				转换后的字符串
		*/
		static AString		Lower( const char* str );
	};

	/** @} */
	/** @} */

}