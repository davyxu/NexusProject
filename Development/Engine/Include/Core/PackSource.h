#pragma once

namespace NekoEngine
{

	class PackGenerator : public FindFileListener
	{
	public:
		PackGenerator( );

		void AddSource( const wchar* path,const wchar* extName = null,bool recursize = false );

		void SetOutputDir( const wchar* path );

		int Generate(const wchar* packfile, Logger* logger, bool nametable = true );

	private:
		virtual bool OnFindFile( const wchar* path, const wchar* name );

		virtual bool OnFindDirectory( const wchar* path , bool& recursive );
		
	private:
		typedef std::list<WString> StringList;
		StringList mFileList;
		WString		mOutpath;
		bool		mRecursize;
	};

	class PackExtractor
	{
	public:
		PackExtractor(const wchar* packFile);
	
		int Extract(const wchar* path, Logger* logger );

	private:
		struct NameTable
		{
			WString mName;
			int mHashName;
		};

		WString mPackfile;
	};
}