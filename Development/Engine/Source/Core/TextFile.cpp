#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{
	TextFile::TextFile()
	: mIsUnicode(false)
	, mDivision(L",;|\t ")
	, mStream(null)
	, mFile( null )
	, mDeleteStream( false )
	{

	}

	TextFile::~TextFile()
	{
		Close();
	}

	void TextFile::Close( )
	{
		if ( mDeleteStream )
		{
			SafeDelete( mStream );
		}

		SafeDelete( mFile );
	}

	bool TextFile::Open( const wchar* filename )
	{
		Assert( mStream == null );

		mStream = mFile = new FileStream( filename, FAM_Read );

		if ( !mStream->IsValid() )
		{
			SafeDelete( mFile );
			return false;
		}

		return Open( mFile, false );
	}

	bool TextFile::Create( const wchar* filename, bool UnicodeFormat)
	{
		Assert( mStream == null );

		mStream = mFile = new FileStream( filename, FAM_Write );

		if ( !mStream->IsValid() )
		{
			SafeDelete( mFile );
			return false;
		}


		return Create( mFile, UnicodeFormat );
	}

	static bool _isunicode( DataStream* stream )
	{
		Assert( stream != null );
		dword size = stream->GetSize();

		if ( size < 2 )
			return false;

		word UnicodeHeader = 0;

		bool IsUnicode = false;
		if ( stream->ReadTypes<word>(UnicodeHeader) && UnicodeHeader == 0xFEFF )
			IsUnicode = true;

		
		if ( !IsUnicode )
			stream->Seek( 0, MSO_Begin );

		return IsUnicode;
	}



	bool TextFile::Open( DataStream* stream, bool copymemory )
	{
		Assert( stream != null );

		if ( copymemory )
			mStream = new HeapStream( stream );
		else
			mStream = stream;

		mDeleteStream = copymemory;
		
		mIsUnicode = _isunicode( stream );
		
		return true;
	}

	bool TextFile::Create(  DataStream* stream, bool UnicodeFormat)
	{
		Assert( stream != null )

		mStream = stream ;
		mDeleteStream = false;

		mIsUnicode = UnicodeFormat;

		if (mIsUnicode)
		{
			mStream->WriteTypes<word>(0xFEFF);
		}

		return true;
	}

	bool TextFile::IsUnicode( )
	{
		return mIsUnicode;
	}

	bool TextFile::WriteLine(const wchar* format, ... )
	{
		Assert(mStream);

		wchar str[ 1024 ];
		StringHelper::FormatBufferV( str, 1024, format, (char*)( &format ) + sizeof( format ) );

		bool num = 0;
		if (mIsUnicode)
		{
			dword len = StringHelper::Length( str );

			if (!mStream->WriteBuffer( str, len * sizeof(wchar) ) )
				return false;

			if (!mStream->WriteBuffer( L"\r\n", 2 * sizeof(wchar) ) )
				return false;
		}
		else
		{
			char* astr = mConv.UnicodeToAnsi(str);
			dword len = StringHelperA::Length( astr );

			if (!mStream->WriteBuffer( astr, len * sizeof(char) ) )
				return false;


			if (!mStream->WriteBuffer( L"\r\n", 2 * sizeof(char) ) )
				return false;
		}

		return true;
	}

	template<typename TChar, typename TString>
	void ReadLineHelper( TString& str, DataStream* m )
	{
		str.clear();

		TChar c;
		TChar peekchar = 0;
		for ( ;; )
		{
			if ( peekchar == 0 )
			{				
				if ( !m->ReadBuffer( &c, sizeof(TChar) ) )
					break;
			}
			else
			{
				c = peekchar;
				peekchar = 0;
			}


			switch ( c )
			{
			case '\n':
				return;
			case '\r':
				continue;
			default:
				str.push_back( c );
			}
		}
	}

	bool TextFile::ReadLine( WString& out )
	{
		Assert( mStream != null );
		if ( IsEnd() )
			return false;

		if ( mIsUnicode )
		{
			ReadLineHelper<wchar, WString>( out, mStream );
		}
		else
		{
			AString str;
			ReadLineHelper<char, AString>( str, mStream );
			out = mConv.AnsiToUnicode( str.c_str() );
		}

		return true;
	}

	bool TextFile::IsEnd()
	{
		Assert( mStream != null );
		return mStream->GetRemainBytes() == 0;
	}

}