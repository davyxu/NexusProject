#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	FileFinder::FileFinder( ) : mFinderHandle( INVALID_HANDLE_VALUE )
	{
	}

	FileFinder::~FileFinder( )
	{
		if ( mFinderHandle != INVALID_HANDLE_VALUE )
			::FindClose( mFinderHandle );
	}

	bool FileFinder::FindFirst( const wchar* filename, FileInfo& fileinfo )
	{
		WIN32_FIND_DATA finddata;
		if ( ( mFinderHandle = ::FindFirstFile( filename, &finddata ) ) == INVALID_HANDLE_VALUE )
			return false;

		fileinfo.mName			= finddata.cFileName;
		fileinfo.mSize			= finddata.nFileSizeLow;
		fileinfo.mAttributes	= finddata.dwFileAttributes;

		return true;
	}

	bool FileFinder::FindNext( FileInfo& fileinfo )
	{
		WIN32_FIND_DATA finddata;
		if ( ::FindNextFile( mFinderHandle, &finddata ) == false )
			return false;

		fileinfo.mName			= finddata.cFileName;
		fileinfo.mSize			= finddata.nFileSizeLow;
		fileinfo.mAttributes	= finddata.dwFileAttributes;

		return true;
	}

	void FileFinder::ScanDirectory( const wchar* dir, FindFileListener* listener  )
	{
		WString pathname = dir;
		Path::NormalizePath( pathname );

		if ( pathname.length( ) > 0 )
			pathname += L"\\";

		FileFinder	filefinder;
		FileFinder::FileInfo	fileinfo;


		if ( filefinder.FindFirst( (pathname + L"*").c_str(), fileinfo ) )
		{
			do
			{
				if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
					continue;

				// Found a file.
				if ( ( fileinfo.mAttributes & FileFinder::Attribute_Directory ) == 0 )
				{

					// Test count only
					if ( listener == null )
						continue;

					// Return false means stop enum
					if ( !listener->OnFindFile( pathname.c_str(), fileinfo.mName.c_str() ) )
						break;
				}
				else
				{
					WString combinepath = pathname + fileinfo.mName;

					if ( (::GetFileAttributes( combinepath.c_str() ) & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN )
						continue;


					if ( listener == null )
						continue;

					bool recursive = false;

					// Return false means stop enum
					if ( !listener->OnFindDirectory( combinepath.c_str(), recursive ) )
						break;

					if ( recursive )
					{						
						ScanDirectory( combinepath.c_str(), listener  );
					}
				}
			}
			// Find next file.
			while ( filefinder.FindNext( fileinfo ) );
		}		
	}
}