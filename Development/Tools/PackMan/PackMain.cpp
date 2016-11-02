
#include "NekoCore.h"
#include "Core/Win32Header.h"
#include "NekoCore_Pack.h"

using namespace NekoEngine;

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")


void print_help( )
{
	Console::WriteLine( L"packman folder    Generate pack file.");
	Console::WriteLine( L"packman pack.ngp    Extrack pack file.");
	Console::WriteLine( L"packman pack.txt -filelist D:\\    Extrack pack file from file list");
}

int wmain(int argc, wchar* argv[])
{
	CmdLineParser parser;
	

	Console::SetLocal();

	if ( !parser.HasArg( 1 ) )
	{
		print_help();
		return 0;
	}

	NEKO_CORE_INIT;
	

	bool wait = true;
	bool gen_nametable = true;
	bool filelist = false;

	const wchar* filelistOutput = L"";

	for( dword i = 2; parser.HasArg( i );i++)
	{
		if ( StringHelper::CompareLowerCase( parser.GetArg( i ), L"-nowait") )
		{
			wait = false;
		}
		else if ( StringHelper::CompareLowerCase( parser.GetArg( i ), L"-nonametable") )
		{
			gen_nametable = false;
		}
		else if ( StringHelper::CompareLowerCase( parser.GetArg( i ), L"-filelist") )
		{
			filelist = true;

			if ( !parser.HasArg( i + 1) )
			{
				Console::WriteLine( L"file list option need output");
				return -1;
			}

			filelistOutput = parser.GetArg( i + 1 );
		}
	}
	
		

	WString filename = parser.GetSafeArg( 1 );
	if ( StringHelper::CompareLowerCase( Path::GetExtention(filename.c_str()), L".ngp" ) )
	{
		PackExtractor pe( filename.c_str() );
		pe.Extract( Path::GetDirectoryName(filename.c_str()).c_str(), &CmdToolLog() );
		if ( wait )
		{
			Console::GetChar();
		}
		return 0;
	}

	if ( filelist )
	{
		PackGenerator gen;
		gen.SetOutputDir( filelistOutput );

		TextFile txtfile;

		if ( !txtfile.Open( filename.c_str() ) )
		{
			Console::WriteLine(L"Can not open file list %s", filename );
			return -1;
		}

		WString cdir = Directory::GetCurrent();

		WString str;
		while( !txtfile.IsEnd() )
		{
			txtfile.ReadLine( str );
			WString dir = Path::GetDirectoryName( str.c_str() );
			WString ext = Path::GetFileName( str.c_str() );
			gen.AddSource( dir.c_str(), ext.c_str(), false );
		}

		

		gen.Generate( filelistOutput , &CmdToolLog(), gen_nametable  );
		
		if ( wait )
		{
			Console::GetChar();
		}
	}
	else
	{
		Path::NormalizePath( filename );
		if ( Directory::Exists( filename.c_str() ) )
		{
			PackGenerator gen;
			gen.SetOutputDir( filename.c_str() );
			gen.AddSource( filename.c_str(), null, true );
			gen.Generate( (WString( filename ) + L".ngp").c_str(), &CmdToolLog(), gen_nametable );
			if ( wait )
			{
				Console::GetChar();
			}
		}
	}



	return 0;
}
