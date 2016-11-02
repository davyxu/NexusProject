#include "CorePCH.h"
#include "NekoCore_Platform.h"


namespace NekoEngine
{


	CmdLineParser::CmdLineParser()
	{
		// Save local copy of the command line string, because
		// ParseCmdLine() modifies this string while parsing it.

		m_cmdline = new wchar [ StringHelper::Length( GetCommandLine() ) + 1];
		if (m_cmdline)
		{
			StringHelper::Copy(m_cmdline, GetCommandLine() );
			ParseCmdLine();
		}
	}

	CmdLineParser::~CmdLineParser()
	{
		delete m_cmdline;
	}

	void CmdLineParser::ParseCmdLine()
	{
		enum { TERM  = '\0',
			QUOTE = '\"' };

		bool bInQuotes = false;
		wchar* pargs = m_cmdline;

		while (*pargs)
		{
			while (iswspace (*pargs))        // skip leading whitespace
				pargs++;

			bInQuotes = (*pargs == QUOTE);  // see if this token is quoted

			if (bInQuotes)                  // skip leading quote
				pargs++;

			mCharArray.push_back (pargs);              // store position of current token

			// Find next token.
			// NOTE: Args are normally terminated by whitespace, unless the
			// arg is quoted.  That's why we handle the two cases separately,
			// even though they are very similar.
			if (bInQuotes)
			{
				// find next quote followed by a space or terminator
				while (*pargs &&
					!(*pargs == QUOTE && (iswspace (pargs[1]) || pargs[1] == TERM)))
					pargs++;
				if (*pargs)
				{
					*pargs = TERM;  // terminate token
					if (pargs[1])   // if quoted token not followed by a terminator
						pargs += 2; // advance to next token
				}
			}
			else
			{
				// skip to next non-whitespace character
				while (*pargs && ! iswspace (*pargs))
					pargs++;
				if (*pargs &&  iswspace (*pargs)) // end of token
				{
					*pargs = TERM;    // terminate token
					pargs++;         // advance to next token or terminator
				}
			}
		} // while (*pargs)
	}

	const wchar* CmdLineParser::GetArg( dword index )
	{
		if ( index >= mCharArray.size() )
			return L"";

		return mCharArray[index];
	}

	bool CmdLineParser::HasArg( dword index )
	{
		if ( index >= mCharArray.size() )
			return false;

		return true;
	}

	NekoEngine::dword CmdLineParser::GetArgCount()
	{
		return mCharArray.size();
	}

	bool CmdLineParser::ArgExist( const wchar* ArgName )
	{
		return GetArgIndex( ArgName ) != -1;
	}

	dword CmdLineParser::GetArgIndex( const wchar* ArgName )
	{
		for ( dword i = 0;i< GetArgCount();i++)
		{
			if ( StringHelper::CompareLowerCase( GetArg(i), ArgName ) )
				return i;
		}

		return -1;
	}

	const wchar* CmdLineParser::GetArgContent( const wchar* ArgName )
	{
		dword ArgIndex = GetArgIndex( ArgName );
		if ( ArgIndex == -1 )
			return L"";

		return GetArg( ArgIndex + 1 );
	}
}