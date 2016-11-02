#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/


	/** 诊断基类
		
	*/
	class Diagnose
	{

	public:
		static void StaticInit( );

		static void StaticExit( );

	protected:

		static void PasteToClipboard( const wchar* information );

		static void ShowErrorDialog( const wchar* information, dword button );

	private:
		static _byte sDebugLock[ 256 ];
	};


	/** 断言
		
	*/
	class ReportAssert : public Diagnose
	{
	public:
		static void ReportError( const wchar* error, const wchar* filename, dword linenumber );
	};



	/** 异常报告
		
	*/

	class ReportException : public Diagnose
	{
	public:

		static void StaticInit( );

		static void StaticExit( );

	private:
		static handle	sOldExceptionFilter;

		static int __stdcall ExceptionFilter( void* exception );

		static bool GetLogicalAddress( void* address, dword& section, dword& offset );
	};

	/** @} */
	/** @} */

};


#ifdef ENABLE_ASSERT
#define Assert( x ) \
{ \
	if ( (x) == 0 ) \
	NekoEngine::ReportAssert::ReportError( L#x, __NFILE__, __NLINE__ ); \
}
#else
#define Assert( x ) \
{ \
}
#endif


