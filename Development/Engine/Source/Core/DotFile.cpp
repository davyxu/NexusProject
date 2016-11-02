#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{
	bool DotFile::Open( DataStream* stream, bool copymemory )
	{
		if (!mTextFile.Open( stream, copymemory ))
			return false;

		return Parse( );
	}

	bool DotFile::Open( const wchar_t* filename )
	{
		if (!mTextFile.Open( filename ))
			return false;

		return Parse( );
	}


	bool DotFile::Parse( )
	{
		WString strLine;
		WString strsect;

		while ( mTextFile.ReadLine(strLine) )
		{
			int first = strLine.find(L'=');
			// µÈÊ½
			if( first == WString::npos)
				continue;

			WString strtmp1= strLine.substr(0,first);
			WString strtmp2= strLine.substr(first+1, WString::npos);
			first= strtmp1.find_first_not_of(L" \t");

			int last = strtmp1.find_last_not_of(L" \t");

			if(first == WString::npos || last == WString::npos)
				continue;

			WString strkey = strtmp1.substr(first, last-first+1);
			first = strtmp2.find_first_not_of(L" \t");

			if(((last = strtmp2.find(L"\t#", first )) != WString::npos) ||
				((last = strtmp2.find(L" #", first )) != WString::npos) ||
				((last = strtmp2.find(L"\t//", first )) != WString::npos)||
				((last = strtmp2.find(L" //", first )) != WString::npos))
			{
				strtmp2 = strtmp2.substr(0, last-first);
			}

			last = strtmp2.find_last_not_of(L" \t");

			if(first == WString::npos || last == WString::npos)
				continue;

			WString strvalue = strtmp2.substr(first, last-first+1);
			mStringMap[strkey]= strvalue;
		}

		return true;
	}



	bool DotFile::Read( const wchar* key, WString& ret )
	{
		StringMap::iterator it = mStringMap.find(WString(key));
		if(it == mStringMap.end())
			return false;

		ret = it->second;
		return true;
	}


}