#include "CorePCH.h"
#include "NekoCore_Platform.h"

//----------------------------------------------------------------------------
// Clipboard Implementation
//----------------------------------------------------------------------------
namespace NekoEngine
{
	bool Clipboard::Copy( const wchar* string )
	{
		if ( ::OpenClipboard( null ) == 0 )
			return false;

		dword size = StringHelper::SizeOfBytes(string);

		HGLOBAL globalmem = ::GlobalAlloc( GHND | GMEM_DDESHARE, size );
		if ( globalmem == null )
		{
			::CloseClipboard( );
			return false;
		}

		::memcpy( ::GlobalLock( globalmem ), (const wchar*) string, size );
		::GlobalUnlock( globalmem );

		::EmptyClipboard( );
		::SetClipboardData( CF_UNICODETEXT, globalmem );

		::GlobalFree( globalmem );
		::CloseClipboard( );

		return true;
	}

	bool Clipboard::Paste( WString& string )
	{
		if ( ::OpenClipboard( null ) == 0 )
			return false;

		HGLOBAL globalmem = null;

		if ( ( globalmem = ::GetClipboardData( CF_UNICODETEXT ) ) != null )
		{
			wchar* unicodetext = (wchar*) ::GlobalLock( globalmem );

			if ( unicodetext != null )
				string = unicodetext;

			::GlobalUnlock( globalmem );
		}
		else if ( ( globalmem = ::GetClipboardData( CF_TEXT ) ) != null )
		{
			char* asciitext = (char*) ::GlobalLock( globalmem );

			if ( asciitext != null )
			{
				StringConverter conv;

				string = conv.AnsiToUnicode( asciitext );
			}

			::GlobalUnlock( globalmem );
		}
		else
		{
			::CloseClipboard( );

			return false;
		}

		::CloseClipboard( );

		return true;
	}
}
