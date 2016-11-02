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
		字符转换类
	*/
	class StringConverter
	{
	public:
		/** 
			释放在转换中分配的内存
		*/
		~StringConverter( );

		/** 
			Ansi到Unicode转换
			@remark
				自动根据需要转换字符串大小分配中间缓冲
			@param
				string 需要转换的字符串
			@param
				number 转换的长度，-1表示转换整个字符串
		*/
		wchar_t*	AnsiToUnicode( const char* string,dword number = -1 );

		/** 
			Unicode到Ansi转换
			@remark
				自动根据需要转换字符串大小分配中间缓冲
			@param
				string 需要转换的字符串
			@param
				number 转换的长度，-1表示转换整个字符串
		*/
		char*		UnicodeToAnsi(const wchar_t* string, dword number = -1 );

		/** 
			UTF8到Unicode转换
			@remark
				自动根据需要转换字符串大小分配中间缓冲
			@param
				string 需要转换的字符串
			@param
				number 转换的长度，-1表示转换整个字符串
		*/
		wchar_t*	UTF8ToUnicode( const char* string, dword number = -1 );

		/** 
			Unicode到UTF8转换
			@remark
				自动根据需要转换字符串大小分配中间缓冲
			@param
				string 需要转换的字符串
			@param
				number 转换的长度，-1表示转换整个字符串
		*/
		char*		UnicodeToUTF8(const wchar_t* string, dword number = -1 );

		/** 
			Ansi到UTF8转换
			@remark
				自动根据需要转换字符串大小分配中间缓冲
			@param
				string 需要转换的字符串
			@param
				number 转换的长度，-1表示转换整个字符串
		*/
		char*		AnsiToUTF8( const char* str );

		/** 
			Ansi到Unicode的转换
			@remark
				提供一个缓冲，将转换结果写入
			@param
				buffer 转换结果的unicode缓冲
			@param
				size 结果缓冲的大小
			@param
				string 需要转换的字符串
		*/
		static wchar* AnsiToUnicode( wchar* buffer, dword size, const char* string );

		/** 
			Unicode到Ansi的转换
			@remark
				提供一个缓冲，将转换结果写入
			@param
				buffer 转换结果的ansi缓冲
			@param
				size 结果缓冲的大小
			@param
				string 需要转换的字符串
		*/
		static char* UnicodeToAnsi( char* buffer, dword size, const wchar* string );

		/** 
			释放转换中分配的内存
		*/
		void		FreeBuffer();

	private:
		std::list<wchar_t*> mUniBufferArray;
		std::list<char*> mAnsiBufferArray;
	};

	/** @} */
	/** @} */
}

