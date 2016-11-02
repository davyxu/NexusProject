#include "CorePCH.h"
#include <DbgHelp.h>
#pragma comment(lib,"dbghelp")

#pragma warning( disable : 4312 ) // warning C4312: 'type cast' : conversion from 'DWORD' to 'void *' of greater size
#pragma warning( disable : 4311 ) // warning C4312: 'type cast' : conversion from 'DWORD' to 'void *' of greater size

namespace NekoEngine
{
	_byte Diagnose::sDebugLock[ 256 ];


	void Diagnose::PasteToClipboard( const wchar* information )
	{
		if ( ::OpenClipboard( null ) == 0 )
			return;

		HGLOBAL globalmem = ::GlobalAlloc( GHND | GMEM_DDESHARE, ( ::wcslen( information ) + 1 ) * sizeof( wchar ) );
		if ( globalmem == null )
			return;

		wchar* buffer = (wchar*) ::GlobalLock( globalmem );
		::wcscpy( buffer, information );
		::GlobalUnlock( globalmem );

		::EmptyClipboard( );
		::SetClipboardData( CF_UNICODETEXT, globalmem );

		::CloseClipboard( );
		::GlobalFree( globalmem );
	}

	void Diagnose::ShowErrorDialog( const wchar* information, dword button )
	{
		int retval = ::MessageBox( ::GetActiveWindow(), information, L"Nexus Engine Diagnose",
			button | MB_TASKMODAL | MB_SETFOREGROUND );

		OutputDebugString( information );

		if ( retval == IDOK || retval == IDABORT )
		{
			::ExitProcess( 3 );
		}
		else if ( retval == IDRETRY )
		{
			__asm int 3
		}
	}

	void Diagnose::StaticInit( )
	{
		::InitializeCriticalSection( (CRITICAL_SECTION*) sDebugLock );
	}

	void Diagnose::StaticExit( )
	{
		::DeleteCriticalSection( (CRITICAL_SECTION*) sDebugLock );
	}


	//----------------------------------------------------------------------------
	// Assert Implementation
	//----------------------------------------------------------------------------

	void ReportAssert::ReportError( const wchar* error, const wchar* filename, dword linenumber )
	{
		wchar buff[1024];
		StringHelper::FormatBuffer(buff,1024,L"Assert Failed : %s\r\n%s:%d\r\n\r\n"
			L"Press ABORT to exit the program immediately.\r\n"
			L"Press RETRY to break if under debugger and abort otherwise.\r\n"
			L"Press IGNORE to ignore the assertion and continue program execution.\r\n"
			, error, filename, linenumber);

		// Paste error information to clipboard.
		//PasteToClipboard( buff );

		// Show the error dialog.
		ShowErrorDialog( buff, MB_ABORTRETRYIGNORE );

	}

	//----------------------------------------------------------------------------
	// Exception Implementation
	//----------------------------------------------------------------------------

	handle	ReportException::sOldExceptionFilter = null;

	int ReportException::ExceptionFilter( void* exception )
	{
		EXCEPTION_POINTERS* exppointer	= (EXCEPTION_POINTERS*) exception;
		EXCEPTION_RECORD*	exprecord	= exppointer->ExceptionRecord;
		CONTEXT*			context		= exppointer->ContextRecord;

		wchar buff[1024];
		StringHelper::FormatBuffer(buff,1024, L"Exception Code : %08X\r\nFault address  : %08X\r\n",
			exprecord->ExceptionCode, exprecord->ExceptionAddress );

		dword section = 0, offset = 0;

		// Get the logical address from the machine address, if found, print the logic address.
		if ( GetLogicalAddress( exprecord->ExceptionAddress, section, offset ) )
		{
			wchar buf6[512];
			StringHelper::FormatBuffer(buf6,512, L"Fault Module   : %04X:%08X\r\n\r\n", section, offset  );

			wcscat(buff,buf6);

		}


		wcscat(buff,L"============================================================\r\n"
			L"Registers\r\n"
			L"============================================================\r\n\r\n");


		wchar buf7[512];
		StringHelper::FormatBuffer(buf7,512,L"CS    : %04X\r\nSS    : %04X\r\nDS    : %04X\r\nES    : %04X\r\nFS    : %04X\r\nGS    : %04X\r\n"
			L"EAX   : %08X\r\nEBX   : %08X\r\nECX   : %08X\r\nEDX   : %08X\r\nESI   : %08X\r\n"
			L"EDI   : %08X\r\nEBP   : %08X\r\nEIP   : %08X\r\nESP   : %08X\r\nFlags : %08X\r\n\r\n",
			context->SegCs, context->SegSs, context->SegDs, context->SegEs, context->SegFs, context->SegGs, context->Eax, context->Ebx,
			context->Ecx, context->Edx, context->Esi, context->Edi, context->Ebp, context->Eip, context->Esp, context->EFlags);
		wcscat(buff,buf7);

		::SymSetOptions( SYMOPT_CASE_INSENSITIVE | SYMOPT_DEFERRED_LOADS );

		if ( ::SymInitialize( ::GetCurrentProcess( ), 0, true ) == false )
		{
			wcscat(buff,L"Can Not Find Debug Information\r\n\r\n");
		}
		else
		{
			wcscat(buff,L"============================================================\r\n"
				L"Call Stack( [Address][Frame][Function][SourceFile] )\r\n"
				L"============================================================\r\n\r\n");

			// Create a frame of stack, prepare to walk stack.
			STACKFRAME stackframe = { 0 };
			stackframe.AddrPC.Offset	= context->Eip;
			stackframe.AddrPC.Mode		= AddrModeFlat;
			stackframe.AddrStack.Offset	= context->Esp;
			stackframe.AddrStack.Mode	= AddrModeFlat;
			stackframe.AddrFrame.Offset	= context->Ebp;
			stackframe.AddrFrame.Mode	= AddrModeFlat;

			while ( true )
			{
				// Get the stack, if cant find, exit walk.
				if ( ::StackWalk( IMAGE_FILE_MACHINE_I386, ::GetCurrentProcess( ), ::GetCurrentThread( ), &stackframe, context, 0, SymFunctionTableAccess, SymGetModuleBase, 0 ) == 0 )
					break;

				if ( stackframe.AddrFrame.Offset == 0 )
					break;

				

				wchar buf2[512];
				StringHelper::FormatBuffer(buf2,512, L"%08X  %08X ", stackframe.AddrPC.Offset, stackframe.AddrFrame.Offset );

				wcscat(buff,buf2);

				_byte symbolbuffer[ sizeof( SYMBOL_INFO ) + 1024 ];
				SYMBOL_INFO* symbol = (SYMBOL_INFO*) symbolbuffer;

				symbol->SizeOfStruct	= sizeof( SYMBOL_INFO ) + 1024;
				symbol->MaxNameLen		= 1024;

				qword displacement = 0;

				// Get symbol from the address.
				if ( ::SymFromAddr( ::GetCurrentProcess( ), stackframe.AddrPC.Offset, (qword*) &displacement, symbol ) )
				{
					// the symbol->Name is ansi, so convert it to unicode.
					StringConverter conv;
					wchar* symbolname = conv.AnsiToUnicode(symbol->Name);

					wchar buf3[512];
					StringHelper::FormatBuffer(buf3,512, L"%s + %08X", symbolname, (dword) displacement);

					wcscat(buff,buf3);

				}
				else
				{
					dword section = 0, offset = 0;

					// Get the logical address from the machine address.
					if ( GetLogicalAddress( (void*) stackframe.AddrPC.Offset, section, offset ) == false )
						break;

					wchar buf4[512];
					StringHelper::FormatBuffer(buf4,512, L"%04X:%08X %s", section, offset );

					wcscat(buff,buf4);
				}

				IMAGEHLP_LINE lineinfo = { sizeof( IMAGEHLP_LINE ) };

				// Get source file name and line number from the address.
				if ( ::SymGetLineFromAddr( ::GetCurrentProcess( ), stackframe.AddrPC.Offset, (dword*) &displacement, &lineinfo ) )
				{
					StringConverter conv;
					// the lineinfo.FileName is ansi, so convert it to unicode.
					wchar* filename = conv.AnsiToUnicode( lineinfo.FileName );


					wchar buf5[512];
					StringHelper::FormatBuffer(buf5,512,  L"\r\n        => %s:%d\r\n", filename, lineinfo.LineNumber );

					wcscat(buff,buf5);
				}
				else
				{
					wcscat(buff,L"\r\n");

				}
			}
		}

		::SymCleanup( ::GetCurrentProcess( ) );

		// Paste error information to clipboard.
		PasteToClipboard( buff );

		// Show the error dialog.
		ShowErrorDialog( buff, MB_OK );
		return EXCEPTION_EXECUTE_HANDLER;
	}

	bool ReportException::GetLogicalAddress( void* address, dword& section, dword& offset )
	{
		MEMORY_BASIC_INFORMATION memoryinfo;

		// Get the virtual address space of the address, if cant find, return false.
		if ( ::VirtualQuery( address, &memoryinfo, sizeof( MEMORY_BASIC_INFORMATION ) ) == 0 )
			return false;

		// Invalid allocation base, return false.
		if ( memoryinfo.AllocationBase > (void*) 0x70000000 )
			return false;

		IMAGE_DOS_HEADER*		dosheader		= (IMAGE_DOS_HEADER*) memoryinfo.AllocationBase;
		IMAGE_NT_HEADERS*		ntheader		= (IMAGE_NT_HEADERS*) ( (_byte*) memoryinfo.AllocationBase + dosheader->e_lfanew );
		IMAGE_SECTION_HEADER*	sectionheader	= IMAGE_FIRST_SECTION( ntheader );

		dword rva = (dword) address - (dword) memoryinfo.AllocationBase, sectionstart = 0, sectionend = 0;

		// Search each section in the PE header, and find the section of the address.
		for ( int i = 0; i < (int) ntheader->FileHeader.NumberOfSections; i ++, sectionheader ++ )
		{
			sectionstart	= sectionheader->VirtualAddress;
			sectionend		= sectionstart + max( sectionheader->SizeOfRawData, sectionheader->Misc.VirtualSize );

			// Found it, assign the value of the section & offset, and return true.
			if ( rva >= sectionstart && rva <= sectionend )
			{
				section	= i + 1;
				offset	= rva - sectionstart;

				return true;
			}
		}

		return false;
	}

	void ReportException::StaticInit( )
	{
		if ( sOldExceptionFilter == null )
			sOldExceptionFilter = ::SetUnhandledExceptionFilter( (PTOP_LEVEL_EXCEPTION_FILTER) ExceptionFilter );
	}

	void ReportException::StaticExit( )
	{
		if ( sOldExceptionFilter != null )
			::SetUnhandledExceptionFilter( (PTOP_LEVEL_EXCEPTION_FILTER) sOldExceptionFilter );
	}
}

