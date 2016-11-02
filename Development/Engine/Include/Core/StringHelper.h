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
	class StringHelper
	{
	public:
		/** 
			格式化字符串
			@param
				format 格式化参数或者字符串
			@return
				格式化后的字符串
		*/
		static WString FormatString( const wchar* format, ... );

		/** 
			格式化字符串，使用栈
			@param
				format 格式化参数或者字符串
			@param
				arguments 参数指针，一般参数为可变参数时，使用(char*)( &format ) + sizeof( format )
			@return
				格式化后的字符串
		*/
		static WString FormatStringV( const wchar* format, const void* arguments );

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
		static const wchar* FormatBuffer( wchar* buffer, size_t size, const wchar* format, ... );

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
		static const wchar* FormatBufferV( wchar* buffer, size_t size, const wchar* format, const void* arguments );

		/** 
			字符串连接
			@remark
				连接字符串1，2， 将结果放置在1当中
			@param
				buffer 字符串1，结果放置缓冲
			@param
				source 字符串2
		*/
		static void Connect(wchar* buffer, const wchar* source);

		/** 
			字符串比较
			@param
				str1 字符串1
			@param
				str2 字符串2
			@return
				字符串是否相等
		*/
		static bool Compare( const wchar* str1, const wchar* str2 );

		/** 
			字符串按照数量比较
			@param
				str1 字符串1
			@param
				str2 字符串2
			@param
				Count 需要比较的字符串数量
			@return
				字符串是否相等
		*/
		static bool Compare( const wchar* str1, const wchar* str2, dword Count );

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
		static void Copy(wchar* buffer,const wchar* source,int size = -1 );

		/** 
			字符串忽略大小写比较
			@param
				str1 字符串1
			@param
				str2 字符串2
			@return
				字符串是否相等
		*/
		static bool CompareLowerCase( const wchar* str1, const wchar* str2 );

		/** 
			获得字符串长度
			@param
				buffer 字符串
			@return
				字符串长度
		*/
		static dword Length( const wchar* buffer );

		/** 
			获取字符串右部分
			@param
				str 输入字符串
			@param
				count 截取数目
			@return
				截取的字符串
		*/
		static WString		Right(const WString& str, size_t count);

		/** 
			获取字符串左部分
			@param
				str 输入字符串
			@param
				count 截取数目
			@return
				截取的字符串
		*/
		static WString		Left(const WString& str, size_t count);

		/** 
			获取字符串中间部分
			@param
				str 输入字符串
			@param
				count 截取数目
			@return
				截取的字符串
		*/
		static WString		Mid( const WString& str, size_t begin,size_t end);

		/** 
			获取字符串占用内存数
			@param
				str 输入字符串
			@return
				字节数
		*/
		static dword		SizeOfBytes( const wchar* str );

		/** 
			去除字符串中左边多余的字符串
			@param
				str 输入字符串
			@param
				delims 去除的部分			
		*/
		static WString		TrimLeft( WString& str ,const wchar* delims = L" \t\r" );

		/** 
			去除字符串中右边多余的字符串
			@param
				str 输入字符串
			@param
				delims 去除的部分			
		*/
		static WString		TrimRight( WString& str ,const wchar* delims = L" \t\r" );

		/** 
			去除字符串中多余的字符串
			@param
				str 输入字符串
			@param
				delims 去除的部分			
		*/
		static WString		Trim( WString& str, const wchar* delims = L" \t\r" );

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
		static int			Parse( const wchar* str, const wchar* format, ... );

		/** 
			将字符串中字母部分大写变小写
			@param
				str 输入字符串
			@return
				转换后的字符串
		*/
		static WString		Lower( const wchar* str );

		/** 
			将字符串中字母部分大写变小写
			@param
				str 输入/输出字符串
		*/
		static void			Lower( WString& str );

		/** 
			替换字符串中的内容
			@param
				str 输入字符串， 结果放置字符串
			@param
				findchar 需要查找的字符
			@param
				replaceto 替换为的字符串
		*/
		static void			Replace( WString& str, wchar findchar, wchar replaceto );

		


		/// 空字符串
		static const WString cBlank;
	};

	

	/** @} */
	/** @} */

}