#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{
	FileStream::FileStream() 
		: mFile(INVALID_HANDLE_VALUE)
		, mFileAccessMode(FAM_Invalid)
	{

	}

	FileStream::FileStream( const wchar* filename, eFileAccessMode accessmode, bool openalways /*= false */ )
		: mFile(INVALID_HANDLE_VALUE)
		, mFileAccessMode(FAM_Invalid)
	{
		Open(filename, accessmode, openalways );
	}

	FileStream::~FileStream()
	{
		Close();
	}


	bool FileStream::Open(const wchar* filename, eFileAccessMode accessmode, bool openalways )
	{
		Assert( mFile == INVALID_HANDLE_VALUE );

#ifdef _DEBUG
		wchar CurrPath[ MAX_PATH ];
		::GetCurrentDirectoryW( MAX_PATH, CurrPath );
#endif

		switch( accessmode )
		{
		case FAM_Read:
			mFile = ::CreateFile( filename, GENERIC_READ,FILE_SHARE_READ,null,openalways ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,null );
			break;
		case FAM_Write:
			mFile = ::CreateFile( filename, GENERIC_WRITE, FILE_SHARE_READ ,null,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,null );
			break;
		default:
			return false;
		}
		
		if ( mFile == INVALID_HANDLE_VALUE )
		{
			return false;
		}

		mFileAccessMode = accessmode;

		return true;
	}

	bool FileStream::IsValid( )
	{
		return mFile != INVALID_HANDLE_VALUE;
	}

	void FileStream::Close()
	{
		::CloseHandle(mFile);
		mFile = INVALID_HANDLE_VALUE;
	}


	bool FileStream::ReadBuffer( void* output, dword numberOfBytesToRead )
	{
		if ( output == null )
			return false;

		if ( GetRemainBytes( ) < numberOfBytesToRead )
			return false;

		dword thisbytes = 0;
		if ( ::ReadFile( mFile, output, numberOfBytesToRead, &thisbytes, null ) == 0 ||
			thisbytes != numberOfBytesToRead )
			return false;

		return true;
	}

	bool FileStream::WriteBuffer( const void* input, dword numberOfBytes )
	{
		Assert( mFileAccessMode == FAM_Write );

		if ( input == null )
			return false;

		dword thisbytes = 0;
		if ( ::WriteFile( mFile, input, numberOfBytes, &thisbytes, null ) == 0||
			thisbytes != numberOfBytes  )
			return false;

		return true;
	}

	bool FileStream::WriteBuffer( DataStream* s )
	{
		return __super::WriteBuffer( s );
	}

	dword FileStream::GetPosition() const
	{
		dword pointer = ::SetFilePointer(mFile,0,null,FILE_CURRENT);
		if (pointer == INVALID_SET_FILE_POINTER)
			return 0;

		return pointer;
	}

	bool FileStream::Seek( int offset, eMemoerySeekOrigin mso )
	{
		DWORD result = INVALID_SET_FILE_POINTER;
		switch( mso )
		{
		case MSO_Begin:
			{
				result = ::SetFilePointer( mFile, offset, null, FILE_BEGIN );
				break;
			}
		case MSO_Current:
			{
				result = ::SetFilePointer( mFile, offset, null, FILE_CURRENT );
				break;
			}
		case MSO_End:
			{
				result = ::SetFilePointer( mFile, offset, null, FILE_END );
				break;
			}
		}

		return result != INVALID_SET_FILE_POINTER;
	}


	dword FileStream::GetRemainBytes( ) const
	{
		dword Size = GetSize();
		dword Position = GetPosition();

		if ( Position < Size )
			return Size - Position;

		return 0;
	}

	dword FileStream::GetSize( ) const
	{
		DWORD hight;
		int filesize = ::GetFileSize(mFile, &hight );

		if (filesize == INVALID_FILE_SIZE)
			return 0;

		return filesize;
	}

	void FileStream::SetLength( dword len )
	{
		Seek( len, MSO_Begin );
		::SetEndOfFile( mFile );
	}

	void FileStream::Reset()
	{
		::SetFilePointer( mFile, 0, null, FILE_BEGIN );
	}

	char* FileStream::GetBuffer()
	{
		/*Assert( "FileStream does not have GetBuffer function\r\n"
			"Try to swap mem.write(file) to mem( file ) or file.Read( mem )" == 0 );*/

		return null;
	}

	char* FileStream::AllocBuffer( dword size )
	{
		Assert( "FileStream can not AllocBuffer\r\n"
				"Try to swap mem.write(file) to mem( file ) or file.Read( mem )" == 0 );

		return null;
	}
}