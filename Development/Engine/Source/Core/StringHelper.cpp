#include "CorePCH.h"
#include <stdio.h>
//#include <algorithm>

namespace NekoEngine
{
	const WString StringHelper::cBlank = L"";

	WString StringHelper::FormatString( const wchar* format, ... )
	{
		Assert( format != null );

		wchar buffer[ 2048 ];
		::_vsnwprintf( buffer, 2048, format, (char*)( &format ) + sizeof( format ) );
		buffer[ 2047 ] = 0;

		return WString( buffer );
	}

	WString StringHelper::FormatStringV( const wchar* format, const void* arguments )
	{
		Assert( format != null && arguments != null );

		wchar buffer[ 2048 ];
		::_vsnwprintf( buffer, 2048, format, (char*) arguments );
		buffer[ 2047 ] = 0;

		return WString( buffer );
	}


	const wchar* StringHelper::FormatBuffer( wchar* buffer, size_t size, const wchar* format, ... )
	{
		Assert( buffer != null && size != 0 && format != null );

		::_vsnwprintf( buffer, size, format, (char*)( &format ) + sizeof( format ) );
		buffer[ size - 1 ] = 0;
		return buffer;
	}

	const wchar* StringHelper::FormatBufferV( wchar* buffer, size_t size, const wchar* format, const void* arguments )
	{
		Assert( buffer != null && size != 0 && format != null && arguments != null );

		::_vsnwprintf( buffer, size, format, (char*) arguments );
		buffer[ size - 1 ] = 0;
		return buffer;
	}

	void StringHelper::Connect(wchar* buffer, const wchar* source)
	{
		::wcscat(buffer,source);
	}

	bool StringHelper::Compare( const wchar* str1, const wchar* str2 )
	{
		return ::wcscmp( str1, str2 ) == 0;
	}

	bool StringHelper::Compare( const wchar* str1, const wchar* str2, dword Count )
	{
		return ::wcsncmp( str1, str2, Count ) == 0;
	}

	void StringHelper::Copy(wchar* buffer,const wchar* source,int size)
	{
		if ( size == -1 )
		{
			::wcscpy( buffer, source );
		}
		else
		{
			::wcsncpy( buffer, source, size );

			buffer[ size ] = 0;
		}
	}


	bool StringHelper::CompareLowerCase( const wchar* str1, const wchar* str2 )
	{
		return ::_wcsicmp( str1, str2 ) == 0;
	}

	dword StringHelper::Length(const wchar* buffer )
	{
		return (dword)::wcslen( buffer );
	}


	WString StringHelper::Mid( const WString& str, size_t begin,size_t end)
	{
		return str.substr( begin, end - begin + 1 );
	}

	WString StringHelper::Left(const WString& str, size_t count)
	{
		return str.substr( 0, count );
	}

	WString StringHelper::Right(const WString& str, size_t count)
	{
		return str.substr( str.length() - count, count );
	}

	dword StringHelper::SizeOfBytes( const wchar* str )
	{
		return ( Length( str ) + 1 ) * sizeof( wchar );
	}

	WString StringHelper::TrimLeft( WString& str, const wchar* delims )
	{		
		return str.erase(0, str.find_first_not_of(delims));
	}

	WString StringHelper::TrimRight( WString& str ,const wchar* delims )
	{
		return str.erase(str.find_last_not_of(delims)+1, str.length() );
	}

	WString StringHelper::Trim( WString& str, const wchar* delims )
	{		
		str.erase(0, str.find_first_not_of(delims));
		str.erase(str.find_last_not_of(delims)+1  );

		return str;
	}
	

	int StringHelper::Parse( const wchar* str, const wchar* format, ... )
	{
		Assert( format != null );

		int paramnumber = 0;

		// Find how many parameters we need.
		for ( int i = 0; format[i] != 0; i ++ )
		{
			if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
				paramnumber ++;
		}

		paramnumber *= sizeof( void* );

		char* parameters = (char*)( &format ) + sizeof( format );

		// Make a local stack, and fill local stack the way swscanf likes it.
		__asm sub esp, paramnumber;
		__asm mov ecx, paramnumber;
		__asm mov esi, parameters;
		__asm mov edi, esp;
		__asm rep movsb;

		int retval = ::swscanf( str, format );

		__asm add esp, paramnumber;

		return retval;
	}

	WString StringHelper::Lower( const wchar* str )
	{
		WString ret = str;
		Lower( ret );
		return ret;
	}

	void StringHelper::Lower( WString& s )
	{
		std::transform(
			s.begin(),
			s.end(),
			s.begin(),
			towlower);
	}

	void StringHelper::Replace( WString& s, wchar FindChar, wchar ReplaceTo)
	{
		for ( dword i = 0;i< s.length();i++)
		{
			if ( s[i] == FindChar )
				s[i] = ReplaceTo;
		}
	}

	//void StringHelper::SplitString( const wchar* source, std::list< WString >& output, const wchar* division, const wchar* trim )
	//{
	//	Assert( Length( division ) > 0 )

	//	dword length = Length( source ), start = -1;

	//	for ( dword i = 0; i < length; i ++ )
	//	{
	//		if ( division( source[i] ) == -1 )
	//		{
	//			// Start of a string.
	//			if ( start == -1 )
	//				start = i;
	//		}
	//		else
	//		{
	//			// End of a string.
	//			if ( start != -1 )
	//			{
	//				// Build candidate string.
	//				WString candidate( (const nChar*) source + start, i - start );

	//				// Trim candidate string.
	//				candidate.Trim( trim );

	//				// It is valid string, can be part of output.
	//				if ( candidate.Length( ) > 0 )
	//					output.Append( candidate );

	//				start = -1;
	//			}
	//		}
	//	}

	//	// End of a string.
	//	if ( start != -1 )
	//	{
	//		// Build candidate string.
	//		WString candidate( (const nChar*) source + start );

	//		// Trim candidate string.
	//		candidate.Trim( trim );

	//		// It is valid string, can be part of output.
	//		if ( candidate.Length( ) > 0 )
	//			output.Append( candidate );

	//		start = -1;
	//	}
	//}


}