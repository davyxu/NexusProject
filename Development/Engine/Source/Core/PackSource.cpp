#include "CorePCH.h"
#include "NekoCore_Stream.h"
#include "NekoCore_Pack.h"


namespace NekoEngine
{
	const wchar* FilePackName = L"ngp_fileinfo";
	const word FilePackVersion = 1;

	PackGenerator::PackGenerator()
		: mRecursize( false )
	{
		
	}

	void PackGenerator::AddSource( const wchar* path,const wchar* extName,bool recursize )
	{
		FileFinder::ScanDirectory( path, this );
		mRecursize = recursize;
	}

	void PackGenerator::SetOutputDir( const wchar* path )
	{
		mOutpath = path;
	}

	bool PackGenerator::OnFindDirectory( const wchar* path , bool& recursive )
	{
		recursive = mRecursize;

		return true;
	}


	bool PackGenerator::OnFindFile( const wchar* path, const wchar* name )
	{
		mFileList.push_back( WString( path ) + name );

		return true;
	}

	int PackGenerator::Generate(const wchar* packfile, Logger* logger, bool nametable )
	{
		if (mFileList.size() == 0)
			return 0;

		PackMaster pm;

		if (!pm.Create(packfile))
		{
			logger->Error(L"can not create output file :%s", packfile);
			return 0;
		}

		dword packedFileCount = 0;
		dword errorcount = 0;
		for(StringList::iterator it = mFileList.begin();
			it != mFileList.end();
			++it )
		{
			WString& filename = *it;			

			WString relativePath = Path::RelativePathTo(mOutpath.c_str(),filename.c_str(), true, false);

			if ( relativePath == L"" )
			{
				relativePath = filename;
			}

			FileStream inputfile;
			if (!inputfile.Open( filename.c_str(), FAM_Read ) )
			{
				logger->Error(L"Can not open input file: %s", filename.c_str() );
				errorcount++;
				continue;
			}

			HeapStream bs( &inputfile );

			if ( pm.WriteChunk( relativePath.c_str(), &bs, nametable ) )
			{
				logger->Info(L"%s", relativePath.c_str() );
			}
			else
			{
				logger->Error(L"Write chunk failed: %s", relativePath.c_str() );
				errorcount++;
				continue;
			}
			
			packedFileCount++;
		}

		StackStream bs;
		bs.WriteTypes<word>( FilePackVersion );
		bs.WriteTypes<dword>( packedFileCount );
		pm.WriteChunk( L"ngp_fileinfo", &bs );

		if ( nametable )
		{
			pm.WriteNameTable();

			logger->Info( L"Name table genreated!");
		}

		logger->Info(L"Total %d files, error: %d", packedFileCount, errorcount );
		logger->Info(L"Pack file: %s", packfile );

		return packedFileCount;
	}


	PackExtractor::PackExtractor(const wchar* packFile)
	{
		mPackfile = packFile;
	}

	int PackExtractor::Extract(const wchar* path, Logger* logger )
	{
		PackMaster pm;

		if (!pm.Open(mPackfile.c_str()))
		{
			logger->Error(L"Can not open pack file: %s",mPackfile);
			return 0;
		}

		{
			StackStream bs;
			if ( !pm.ReadChunkByName( FilePackName, &bs ) )
			{
				logger->Error( L"Invalid file pack" );
				return 0;
			}
		}

		
		if ( !pm.ResetChunkIterator() )
		{
			logger->Error( L"Pack dose not contain name table" );
			return 0;
		}

		WString filename;
		HeapStream bs;

		WString outputfolder = Path::Combine( path, Path::GetFileNameWithoutExtension(mPackfile.c_str()).c_str() );

		dword numFileExtract = 0;
		dword errorcount = 0;
		while( pm.IteratorChunk( filename, &bs ) )
		{
			

			FileStream outputfile;

			WString outfilename = Path::Combine( outputfolder.c_str(), filename.c_str() );

			Directory::Create(Path::GetDirectoryName(outfilename.c_str()).c_str());

			if ( !outputfile.Open( outfilename.c_str(),FAM_Write ) )
			{
				logger->Error( L"Create file failed: %s", outfilename.c_str() );
				errorcount++;
				bs.Reset();
				continue;
			}
			
			outputfile.WriteBuffer( &bs );

			logger->Info(L"%s", outfilename.c_str() );

			numFileExtract++;

			bs.Reset();
		}

		logger->Info(L"Total %d files, error: %d", numFileExtract, errorcount );
		logger->Info(L"Pack file: %s", mPackfile.c_str() );

		return numFileExtract;
	}
}

