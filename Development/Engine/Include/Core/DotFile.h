#pragma once


namespace NekoEngine
{
	class DotFile
	{
	public:
		bool Open( DataStream* stream, bool copymemory  );

		bool Open( const wchar_t* filename );

		WString Read( const wchar* key )
		{
			WString ret;

			Read( key, ret );

			return ret;
		}

		template<typename T> 
		bool Read( const wchar* key, T& v )
		{
			WString ret;

			if (!Read( key, ret ) )
				return false;

			if ( !value_parse( ret, v) )
				return false;

			return true;
		}

		template<typename T> 
		T Read( const wchar* key )
		{
			T v;
			if ( Read( key, v ) )
				return v;

			return value_default<T>( );
		}

		template<typename T> 
		T Read( const wchar* key, const T& defaultvalue )
		{
			T v;
			if ( Read( key, v ) )
				return v;

			return defaultvalue;
		}

		bool Read( const wchar* key, WString& ret );

	private:
		bool Parse( );

	private:
		TextFile mTextFile;
		typedef std::map<WString, WString, std::less<WString> > StringMap;
		StringMap mStringMap;
	};


	//DotFileFromFile file;
	//file.Open(L"a.config" );
	//file.Parse();
	//int str = file.Read<int>( L"System.Count" );
}