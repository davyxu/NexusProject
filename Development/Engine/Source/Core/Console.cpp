#include "CorePCH.h"
#include <locale.h>
#include <stdio.h>
#include <stdarg.h>

namespace NekoEngine
{
	void Console::SetLocal(const char* localstr)
	{
		if (localstr == null)
			::setlocale(LC_ALL,"Chinese-simplified");
		else
			::setlocale(LC_ALL,localstr);
	}

	int Console::Write( const wchar* format, ... )
	{
		return ::vwprintf( format, (char*)( &format ) + sizeof( format ) );
	}

	int Console::WriteLine(  const wchar* format, ...  )
	{
		int ret = ::vwprintf( format, (char*)( &format ) + sizeof( format ) );
		::wprintf( L"\n");
		return ret;
	}

	int Console::Input( const wchar* format, ... )
	{
		int paramnumber = 0;

		// Find how many parameters we need.
		for ( int i = 0; format[i] != 0; i ++ )
		{
			if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
				paramnumber ++;
		}

		paramnumber *= sizeof( void* );

		char* parameters = (char*)( &format ) + sizeof( format );

		// Make a local stack, and fill local stack the way wscanf likes it.
		__asm sub esp, paramnumber;
		__asm mov ecx, paramnumber;
		__asm mov esi, parameters;
		__asm mov edi, esp;
		__asm rep movsb;

		int retval = ::wscanf( format );

		__asm add esp, paramnumber;

		return retval;
	}

	wchar Console::GetChar( )
	{
		return ::getwchar( );
	}

	bool Console::SetConsoleEventCallBack( ConsoleEvent handle )
	{
		//BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
		//{
		//	switch (ctrl_type)
		//	{
		//	case CTRL_C_EVENT:
		//	case CTRL_BREAK_EVENT:
		//	case CTRL_CLOSE_EVENT:
		//	case CTRL_SHUTDOWN_EVENT:
		//		console_ctrl_function();
		//		return TRUE;
		//	default:
		//		return FALSE;
		//	}
		//}


		return ::SetConsoleCtrlHandler( (PHANDLER_ROUTINE)handle, true ) != 0;
	}

}

