#include "CorePCH.h"
#include <stdio.h>
#include <shlwapi.h>
#include "NekoCore_Platform.h"

#pragma comment(lib,"shlwapi.lib")

namespace NekoEngine
{
	WString Path::RemoveExtention( const wchar* fullpath )
	{
		wchar buffer[1024];
		StringHelper::Copy( buffer, fullpath );
		::PathRemoveExtensionW( buffer );

		return WString( buffer );
	}


	WString Path::RenameExtension( const wchar* fullpath, const wchar* NewExtName )
	{
		wchar buffer[1024];
		StringHelper::Copy( buffer, fullpath );
		::PathRenameExtension( buffer, NewExtName );

		return WString( buffer );
	}


	bool Path::IsDirecty( const wchar* fullpath )
	{
		return ::PathIsDirectoryW( fullpath ) != FALSE;
	}

	bool Path::IsRelative( const wchar* fullpath )
	{
		return ::PathIsRelativeW( fullpath ) != FALSE;
	}

	bool Path::IsFullpath( const wchar* fullpath )
	{
		if ( StringHelper::Length(fullpath) < 2 )
			return false;

		// \\fileserver\test\abc.txt, that is a full path.
		if ( fullpath[0] == L'\\' && fullpath[1] == L'\\' )
			return true;

		if ( fullpath[1] != L':' )
			return false;

		// D:\Project\abc.txt, that is a full path too.
		if ( fullpath[0] >= L'A' && fullpath[0] <= L'Z' )
			return true;

		// d:\Project\abc.txt, that is a full path too.
		if ( fullpath[0] >= L'a' && fullpath[0] <= L'z' )
			return true;

		return false;
	}

	WString Path::GetFileName( const wchar* fullpath )
	{
		wchar buffer[1024];
		StringHelper::Copy( buffer, fullpath );

		::PathStripPath( buffer );

		return WString( buffer );
	}

	WString Path::GetFileNameWithoutExtension( const wchar* fullpath )
	{
		wchar buffer[1024];
		StringHelper::Copy( buffer, fullpath );

		::PathStripPath( buffer );

		::PathRemoveExtensionW( buffer ) ;

		return WString( buffer );
	}

	const wchar* GetPathName( wchar* buffer, size_t size, const wchar* fullpath, bool wholepath )
	{
		Assert( buffer != null && size != 0 );

		size_t i = -1;

		WString strFullPath(fullpath);

		if ( wholepath == true )
		{
			i = strFullPath.rfind( '\\' );

			if ( i == -1 )
				i = strFullPath.rfind( '/' );

			if ( i == -1 )
				i = strFullPath.rfind( ':' );
		}
		else
		{
			i = strFullPath.find( '\\' );

			if ( i == -1 )
				i = strFullPath.find( '/' );

			if ( i == -1 )
				i = strFullPath.find( ':' );
		}

		// Calculate the size of string to be copy.
		size_t length = i == -1 ? 0 : i;

		if ( length >= size )
			length = size - 1;

		::memcpy( buffer, (const wchar*) fullpath, length * sizeof( wchar ) );
		buffer[ length ] = 0;

		return buffer;
	}


	WString Path::GetDirectoryName( const wchar* fullpath, bool wholepath )
	{
		wchar buffer[1024];

		return WString( GetPathName( buffer, 1024, fullpath, wholepath ) );
	}




	void Path::NormalizePath( WString& path )
	{
		size_t finalpos = path.length() - 1;

		if ( finalpos == WString::npos )
			return;

		size_t i = path.rfind( '\\' );
		if ( i == finalpos )
		{
			path.erase( path.begin() + finalpos );
			return;
		}

		i = path.rfind( '/' );
		if ( i == finalpos )
		{
			path.erase( path.begin() + finalpos );
			return;
		}
	}

	WString Path::Append( const wchar* path, const wchar* anotherpath )
	{
		wchar buffer[1024];
		StringHelper::Copy( buffer, path );
		if (!::PathAppendW( buffer, anotherpath ) )
			return L"";

		return WString( buffer );
	}

	const wchar* Path::GetExtention( const wchar* fullpath )
	{
		return ::PathFindExtensionW( fullpath );
	}
	WString Path::RelativePathTo( const wchar* from, const wchar* to, bool fromisdir, bool toisdir )
	{
		wchar buffer[1024];
		if (!PathRelativePathToW( buffer, 
							 from,
							 fromisdir ? FILE_ATTRIBUTE_DIRECTORY: FILE_ATTRIBUTE_NORMAL,
							 to,
							 toisdir ? FILE_ATTRIBUTE_DIRECTORY: FILE_ATTRIBUTE_NORMAL ))
			return L"";

		if ( buffer[0] == '.' && buffer[1] == '\\')
			return WString( buffer + 2 );

		return WString( buffer );
	}

	WString Path::Combine( const wchar* path, const wchar* file )
	{
		WString strpath( path );
		WString strfile( file );

		return strpath + L"\\" + strfile;
	}
}