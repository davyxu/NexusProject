#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/

	/** 冗余校验
		
	*/

	class CheckSum
	{
	public:
		/// 全局初始化
		static void StaticInit( );

		/** 
			内存循环CRC冗余校验	
			@param
				InData 输入内存
			@param
				Length 内存长度
			@param
				CRC 可选的一个初始CRC值
		*/ 
		static dword	MemCRC( const void* InData, int Length, dword CRC = 0 );

		/// Unicode字符串Hash操作
		static dword	StrHash( const wchar* Data );

		/// ANSI字符串Hash操作
		static dword	StrHash( const char* Data );

		/// Unicode字符串Hash操作，大小写不敏感
		static dword	StrHashNoCase(const wchar* Data);

		/// ANSI字符串Hash操作，大小写不敏感
		static dword	StrHashNoCase(const char* Data );

		
		/** 
			暴雪用，Unicode字符串Hash操作
			@remarks
				暴雪系列字符串重复率低，效率高
			@param
				str 输入字符串
			@param
				type 0 为hash索引， 1和2为compare key
		*/
		static dword	BLZStrHash(const wchar* str, dword type );

		/** 
			暴雪用，ANSI字符串Hash操作，大小写不敏感
			@remarks
				暴雪系列字符串重复率低，效率高
			@param
				str 输入字符串
			@param
				type 0 为hash索引， 1和2为compare key
		*/
		static dword	BLZStrHash(const char* str, dword type );

	private:
		static void BLZStaticInit( );

	private:
		static bool mInitialized;

		// Blizzard Powerful Hash stuff
		enum{ BLZBUFFER_SIZE = 0x500 };

		typedef dword	BLZBuffer[BLZBUFFER_SIZE];

		static BLZBuffer mBLZBuffer;
	};

	/** @} */
	/** @} */
}
