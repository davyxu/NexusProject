#include "CorePCH.h"

#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	static inline bool IsIdentifier(const char* p)
	{
		return (*p>='a' && *p<='z') || (*p>='A' && *p<='Z') || *p == '_' || (*p>='0' && *p<='9');
	}

	SimpleLexer::SimpleLexer( const char* buff, int size )
		: mLexerListener( null )
	{
		if ( size == 0 )
		{
			Error( L"Buffer size = 0 ");
		}

		mSource			= new char[size + 1];
		::memcpy( (void*)mSource, buff, sizeof(char)* size );
		mSource[size] = 0;
		mSize			= size;
		mPointer		= mSource;
		mSwapBuffer[0]	= 0;
		mTokenType		= TT_Unknown;
		mNumber			= 0.0f;
		mLexerListener	= null;
	}

	void SimpleLexer::Error( const wchar* error )
	{
		if ( mLexerListener )
			mLexerListener->OnError( L"Buffer size = 0", mLineNumber, mPointer );
	}

	SimpleLexer::~SimpleLexer( )
	{
		SafeDeleteArray( mSource );
	}

	bool SimpleLexer::Match( const char* token )
	{
		if ( mTokenType != TT_Identifier )
		{
			Error( L"Expect identifier");
			return false;
		}

		return ::strcmp( token, mSwapBuffer ) == 0;
	}

	const char* SimpleLexer::GetString()
	{
		return mSwapBuffer;
	}

	bool SimpleLexer::ReadNumber( )
	{
		char*& p = mPointer;

		if ( *p<'0' || *p>'9' )
			return false;

		mTokenType = TT_Number;

		int i = 0;
		for( i= 0; (*p>='0' && *p<='9') || *p=='.' && i< MaxValueBuffer; i++)
			mSwapBuffer[i]=*p++;

		Assert( i < MaxValueBuffer);
		mSwapBuffer[i] = '\0';

		mNumber = (float)::atof(mSwapBuffer);


		return true;
	}

	void SimpleLexer::ReadWhiteSpace( )
	{
		char*& p = mPointer;

		// Tim space and table
		while(*p==' ' || *p=='\t' || *p=='\n' || *p=='\r')
		{
			if(*p=='\n')
				mLineNumber++;

			p++;
		}
	}

	void SimpleLexer::ReadComment( )
	{
		char*& p = mPointer;

Reparse:
		if ( *p != '/')
			return;

		// Skip first /
		p++;

		// Line comment
		if (*p == '/' )
		{
			// Skip /
			p++;

			// Skip text
			while(*p && *p!='\n')
				p++;

			// Direct to next valid char
			ReadWhiteSpace( );

			goto Reparse;
		}
		// Block comment start
		else if (*p == '*')
		{
			p++;

			// Skip text
			while( *p && !(*p == '*' && *(p + 1) && *(p + 1) == '/'))
			{
				// Count new line in comment
				if(*p=='\n')
					mLineNumber++;

				p++;
			}

			p+=2;

			// Direct to next valid char
			ReadWhiteSpace(  );

			goto Reparse;
		}
	}

	bool SimpleLexer::ReadIdentifier( )
	{
		char*& p = mPointer;

		if ( !IsIdentifier( p ) )
			return false;


		int i = 0;
		for ( i = 0; IsIdentifier( p ) && i< MaxValueBuffer;i++)
			mSwapBuffer[i]=*p++;

		Assert( i < MaxValueBuffer);
		mSwapBuffer[i] = '\0';

		mTokenType = TT_Identifier;

		return true;
	}


	bool SimpleLexer::ReadString( )
	{
		char*& p = mPointer;

		if(*p!='"')
			return false;

		// Skip left quote
		p++;

		char* strptr = p;

		int i = 0;
		// Detect length of string
		for( i = 0; *strptr!='"' ; i++,strptr++)
		{
			if ( *strptr == 0 || *strptr =='\n'|| *strptr =='\r')
			{
				return false;
				break;
			}
		}

		i = Math::Min( i, (int)MaxValueBuffer );

		::strncpy( mSwapBuffer,p,i);
		mSwapBuffer[i] = 0;

		// Step the end of string
		p += i;

		mTokenType = TT_String;

		// Skip right quote
		if(*p)
			p++;

		return true;
	}


	void SimpleLexer::ReadUnknown( )
	{
		mTokenType = TT_Unknown;

		char*& p = mPointer;

		int i = 0;
		for ( i = 0; *p && *p!=' ' && *p!='\t' && *p!='\n' && *p!='\r' && i< MaxValueBuffer;i++)
			mSwapBuffer[i]= *p++;

		Assert( i < MaxValueBuffer );
		mSwapBuffer[ i ] = '\0';
	}

	bool SimpleLexer::ReadDelimiter( )
	{
		static const char* delimiter = ".,()[]{}=<>+-*/\\|^%%=&#";

		for ( char* p = (char*)delimiter;p != 0;p++)
		{
			if ( *p == *mPointer )
			{
				mSwapBuffer[0] = *p;
				mSwapBuffer[1] = 0;
				mPointer++;
				return true;
			}
		}

		return false;
	}

	bool SimpleLexer::Next()
	{
		char*& p = mPointer;

		if ( p == null )
		{
			mSwapBuffer[0] = 0;
			return false;
		}

		ReadWhiteSpace( );

		ReadComment( );

		// End of text
		if (!*p)
		{
			mSwapBuffer[0] = 0;
			return false;
		}

		// Number
		if ( ReadNumber( ) )
			return true;

		if ( ReadIdentifier( ) )
			return true;

		// Quoted string
		if ( ReadString( ) )
			return true;

		if ( ReadDelimiter( ) )
			return true;


		ReadUnknown();

		Error( L"Unknown character");

		return false;
	}

	float SimpleLexer::GetNumber()
	{
		return mNumber;
	}

	int SimpleLexer::GetInteger()
	{
		return (int)mNumber;
	}

	SimpleLexer::TokenType SimpleLexer::GetType()
	{
		return mTokenType;
	}

	void SimpleLexer::SetListener( LexerListener* listener )
	{
		mLexerListener = listener;
	}
}