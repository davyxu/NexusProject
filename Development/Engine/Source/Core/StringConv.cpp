#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	StringConverter::~StringConverter( )
	{
		FreeBuffer();
	}

	wchar_t* StringConverter::AnsiToUnicode( const char* string,dword number )
	{
		Assert( string != null );

		// See how big we need the unicode buffer to be.
		dword size = ::MultiByteToWideChar( CP_ACP, 0, string, number, null, 0 );

		// Create the buffer and fill it.
		wchar_t* buffer = new wchar_t[ size + 1 ];

		mUniBufferArray.push_back(buffer);


		::MultiByteToWideChar( CP_ACP, 0, string, number, buffer, size );
		buffer[ size ] = 0;

		return buffer;
	}

	char* StringConverter::UnicodeToAnsi(const wchar_t* string, dword number )
	{
		Assert( string != null );

		// See how big we need the ansi buffer to be.
		dword size = ::WideCharToMultiByte( CP_ACP, 0, string, number, null, 0, null, null );

			// Create the buffer and fill it.
		char* buffer = new char[ size + 1 ];

		mAnsiBufferArray.push_back( buffer );

		::WideCharToMultiByte( CP_ACP, 0, string, number, buffer, size, null, null );
		buffer[ size ] = 0;

		return buffer;
	}

	wchar* StringConverter::AnsiToUnicode( wchar* buffer, dword size, const char* string )
	{
		Assert( string != null )

		// See how big we need the unicode buffer to be.
		dword length = ::MultiByteToWideChar( CP_ACP, 0, string, -1, null, 0 );

		length = Math::Min( length, size - 1 );


		::MultiByteToWideChar( CP_ACP, 0, string, -1, buffer, length );
		buffer[ length ] = 0;

		return buffer;
	}

	char* StringConverter::UnicodeToAnsi( char* buffer, dword size, const wchar* string )
	{
		Assert( string != null )

		// See how big we need the ansi buffer to be.
		dword length = ::WideCharToMultiByte( CP_ACP, 0, string, -1, null, 0, null, null );

		length = Math::Min( length, size - 1 );


		::WideCharToMultiByte( CP_ACP, 0, string, -1, buffer, length, null, null );
		buffer[ length ] = 0;

		return buffer;
	}
	wchar_t* StringConverter::UTF8ToUnicode( const char* string,dword number )
	{
		Assert( string != null );

		// See how big we need the unicode buffer to be.
		dword size = ::MultiByteToWideChar( CP_UTF8, 0, string, number, null, 0 );

		// Create the buffer and fill it.
		wchar_t* buffer = new wchar_t[ size + 1 ];

		mUniBufferArray.push_back(buffer);


		::MultiByteToWideChar( CP_UTF8, 0, string, number, buffer, size );
		buffer[ size ] = 0;

		return buffer;
	}

	char* StringConverter::UnicodeToUTF8(const wchar_t* string, dword number )
	{
		Assert( string != null );

		// See how big we need the ansi buffer to be.
		dword size = ::WideCharToMultiByte( CP_UTF8, 0, string, number, null, 0, null, null );

		// Create the buffer and fill it.
		char* buffer = new char[ size + 1 ];

		mAnsiBufferArray.push_back( buffer );

		::WideCharToMultiByte( CP_UTF8, 0, string, number, buffer, size, null, null );
		buffer[ size ] = 0;

		return buffer;
	}

	char* StringConverter::AnsiToUTF8( const char* str )
	{
		return UnicodeToUTF8( AnsiToUnicode( str ) );
	}


	void StringConverter::FreeBuffer( )
	{
		for (std::list<char*>::iterator it = mAnsiBufferArray.begin();
			it != mAnsiBufferArray.end();
			it++)
		{
			char* p = *it;
			SafeDeleteArray(p);
		}

		mAnsiBufferArray.clear();

		for (std::list<wchar_t*>::iterator it = mUniBufferArray.begin();
			it != mUniBufferArray.end();
			it++)
		{
			wchar_t* p = *it;
			SafeDeleteArray(p);
		}

		mUniBufferArray.clear();
	}


}

