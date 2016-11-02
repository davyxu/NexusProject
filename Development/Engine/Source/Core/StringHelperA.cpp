#include "CorePCH.h"
#include <stdio.h>


namespace NekoEngine
{
	AString StringHelperA::FormatString( const char* format, ... )
	{
		Assert( format != null );

		char buffer[ 2048 ];
		::_vsnprintf( buffer, 2048, format, (char*)( &format ) + sizeof( format ) );
		buffer[ 2047 ] = 0;

		return AString( buffer );
	}

	AString StringHelperA::FormatStringV( const char* format, const void* arguments )
	{
		Assert( format != null && arguments != null );

		char buffer[ 2048 ];
		::_vsnprintf( buffer, 2048, format, (char*) arguments );
		buffer[ 2047 ] = 0;

		return AString( buffer );
	}


	const char* StringHelperA::FormatBuffer( char* buffer, size_t size, const char* format, ... )
	{
		Assert( buffer != null && size != 0 && format != null );

		::_vsnprintf( buffer, size, format, (char*)( &format ) + sizeof( format ) );
		buffer[ size - 1 ] = 0;
		return buffer;
	}

	const char* StringHelperA::FormatBufferV( char* buffer, size_t size, const char* format, const void* arguments )
	{
		Assert( buffer != null && size != 0 && format != null && arguments != null );

		::_vsnprintf( buffer, size, format, (char*) arguments );
		buffer[ size - 1 ] = 0;
		return buffer;
	}

	void StringHelperA::Connect(char* buffer, const char* source)
	{
		::strcat(buffer,source);
	}

	bool StringHelperA::Compare( const char* str1, const char* str2 )
	{
		return ::strcmp( str1, str2 ) == 0;
	}

	void StringHelperA::Copy(char* buffer,const char* source,int size)
	{
		if ( size == -1 )
		{
			::strcpy( buffer, source );
		}
		else
		{
			::strncpy( buffer, source, size );

			buffer[ size ] = 0;
		}
	}


	bool StringHelperA::CompareLowerCase( const char* str1, const char* str2 )
	{
		return ::_stricmp( str1, str2 ) == 0;
	}

	dword StringHelperA::Length(const char* buffer )
	{
		return (dword)::strlen( buffer );
	}

	AString StringHelperA::Mid( const AString& str, size_t begin,size_t end)
	{
		return str.substr( begin, end - begin + 1 );
	}

	AString StringHelperA::Left(const AString& str, size_t count)
	{
		return str.substr( 0, count );
	}

	AString StringHelperA::Right(const AString& str, size_t count)
	{
		return str.substr( str.length() - count, count );
	}

	dword StringHelperA::SizeOfBytes( const char* str )
	{
		return ( Length( str ) + 1 ) * sizeof( char );
	}

	AString StringHelperA::TrimLeft( const char* str ,const char* delims )
	{
		AString strstr(str);
		strstr.erase(0, strstr.find_first_not_of(delims)+1);
		return strstr;
	}

	AString StringHelperA::TrimRight( const char* str ,const char* delims )
	{
		AString strstr(str);
		strstr.erase(strstr.find_last_not_of(delims)+1, strstr.length() );
		return strstr;
	}

	int StringHelperA::Parse( const char* str, const char* format, ... )
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

		int retval = ::sscanf( str, format );

		__asm add esp, paramnumber;

		return retval;
	}

	AString StringHelperA::Lower( const char* str )
	{
		AString ret = str;
		std::transform(
			ret.begin(),
			ret.end(),
			ret.begin(),
			tolower);
		return ret;
	}


	//void StringHelperA::SplitString( const char* source, std::list< AString >& output, const char* division, const char* trim )
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
	//				AString candidate( (const nChar*) source + start, i - start );

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
	//		AString candidate( (const nChar*) source + start );

	//		// Trim candidate string.
	//		candidate.Trim( trim );

	//		// It is valid string, can be part of output.
	//		if ( candidate.Length( ) > 0 )
	//			output.Append( candidate );

	//		start = -1;
	//	}
	//}


}