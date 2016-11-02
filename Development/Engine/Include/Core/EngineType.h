#pragma once

// 字符支持
#define UTEXT(x) L##x
#define UNICODE_WIDEN(x) UTEXT(x)

#define VAR_TO_STRING( Var ) UTEXT(#Var)

#define __NFILE__ UNICODE_WIDEN(__FILE__)
#define __NFUNCION__ UNICODE_WIDEN(__FUNCTION__)
#define __NLINE__ __LINE__

#define SafeRelease(p) {if(p) {(p)->Release();(p)= 0;}}
#define ArraySize(arr) (sizeof(arr)/sizeof(arr[0]))

#define SafeDelete(p) {if(p) {delete (p);(p)= 0 ;}}
#define SafeDeleteArray(p) {if (p){delete [](p);(p) =0;}}

#define null 0

#define StructOffset( struc, member )	( (unsigned long)&((struc*)0)->member )


#define ENGINE_FOURCC( ch0, ch1, ch2, ch3 ) \
	( (NekoEngine::dword)(NekoEngine::_byte)(ch0) | ( (NekoEngine::dword)(NekoEngine::_byte)(ch1) << 8 ) |	\
	( (NekoEngine::dword)(NekoEngine::_byte)(ch2) << 16 ) | ( (NekoEngine::dword)(NekoEngine::_byte)(ch3) << 24 ) )


namespace NekoEngine
{
	typedef unsigned long dword;
	typedef unsigned char _byte;
	typedef unsigned short word;
	typedef unsigned __int64 qword;
	typedef void* handle;
	
#ifdef _NATIVE_WCHAR_T_DEFINED
	typedef wchar_t wchar;				// C++编译选项 将wchar_t 视为内建类型，VC2005以后均使用
#else
	typedef unsigned short wchar;		// C++编译选项 不将wchar_t 视为内建类型，兼容Qt链接时参数类型,兼容VC2003以前版本
#endif

	typedef std::wstring WString;
	typedef std::string AString;

	template<class T>
	inline void type_constructor(T* p)
	{
		new(p)T;
	}

	template<class T>
	inline void type_destructor(T* p)
	{
		p->~T();
	}
}