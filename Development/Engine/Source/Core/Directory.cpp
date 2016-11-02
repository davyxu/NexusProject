#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	//Directory
	//////////////////////////////////////////////////////////////////////////
	bool Directory::Create( const wchar* pathname )
	{
		Assert( pathname);

		if ( pathname[ StringHelper::Length( pathname ) - 1 ] == ':' )
				return true;

		// Auto create middle path.
		for ( int i = 0; pathname[i] != 0; i ++ )
		{
			if ( pathname[i] != '\\' && pathname[i] != '/' )
				continue;

			if ( i == 0 )
				continue;

			if ( pathname[ i - 1 ] == ':' )
				continue;

			WString middlepath( pathname, i );

			if ( ::GetFileAttributes( middlepath.c_str() ) != -1 )
				continue;

			if ( ::CreateDirectory( middlepath.c_str(), null ) == false )
				return false;
		}

		return ::CreateDirectory( pathname, null ) != false;
	}

	bool Directory::Exists(const wchar* filename)
	{
		Assert(filename);
		dword attrib = ::GetFileAttributes( filename );

		return attrib != -1 && ( attrib & FILE_ATTRIBUTE_DIRECTORY ) != 0;
	}

	const wchar* Directory::GetFullPath(const wchar* file)
	{
		static wchar buff[MAX_PATH];

		::GetFullPathName(file,MAX_PATH,buff, null);
		return buff;
	}


	const wchar* Directory::GetCurrent(void)
	{
		static wchar buff[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH,buff);
		return buff;
	}

	void Directory::SetCurrent(const wchar* path)
	{
		::SetCurrentDirectory(path);
	}

	bool Directory::IsWritable( const wchar* PathName )
	{
		wchar Buffer[MAX_PATH];
		if (!GetTempFileName( PathName, L"DI", 0, Buffer ))
			return false;

		FileStream TestFile( Buffer, FAM_Write );
		bool CanWrite = TestFile.IsValid();
		
		TestFile.Close();

		File::Delete( Buffer );

		return CanWrite;
	}

	bool Directory::IsHidden( const wchar* FileName )
	{
		return (::GetFileAttributes( FileName ) & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
	}

	//////////////////////////////////////////////////////////////////////////
	//DirectoryStack
	//////////////////////////////////////////////////////////////////////////

	DirectoryStack::DirectoryStack(const wchar* newPath)
	{
		Push( newPath );
	}

	DirectoryStack::DirectoryStack( )
	{

	}

	DirectoryStack::~DirectoryStack()
	{
		Directory::SetCurrent(mOldDirectory.c_str());
	}

	void DirectoryStack::Caputure(  )
	{
		mOldDirectory = Directory::GetCurrent();
	}

	void DirectoryStack::Push( const wchar* newPath )
	{
		Directory::SetCurrent(newPath);
	}

}