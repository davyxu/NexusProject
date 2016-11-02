#include "CorePCH.h"
#include <shlwapi.h>
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	bool File::Exists(const wchar* FileName)
	{
		return ::PathFileExists( FileName ) != FALSE;
	}

	dword  File::GetFileSize( const wchar* FileName )
	{
		HANDLE FileHandle = ::CreateFile( FileName, GENERIC_READ,FILE_SHARE_READ,null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( FileHandle == INVALID_HANDLE_VALUE)
			return 0;

		dword filesize = ::GetFileSize(FileHandle, null );

		if (filesize == INVALID_FILE_SIZE)
			return 0;

		::CloseHandle( FileHandle );

		return filesize;
	}

	bool File::GetFileModifyTime( const wchar* FileName, FileTime& Time )
	{
		HANDLE FileHandle = ::CreateFile( FileName, GENERIC_READ,FILE_SHARE_READ,null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( FileHandle == INVALID_HANDLE_VALUE)
			return false;

		bool Result = true;

		if (!::GetFileTime( FileHandle, null, null, (LPFILETIME)&Time ))
		{
			Result = false;
		}

		::CloseHandle(FileHandle);

		return Result;
	}

	bool File::Delete( const wchar* FileName )
	{
		return ::DeleteFile( FileName ) != FALSE;
	}

	bool File::IsHidden( const wchar* FileName )
	{
		return (::GetFileAttributes( FileName ) & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
	}

}