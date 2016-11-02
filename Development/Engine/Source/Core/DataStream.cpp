#include "CorePCH.h"
#include "NekoCore_Stream.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	DataStream::DataStream( )
		: mDataSize( 0 )
		, mPointer( 0 )
		, mAllocedSize( 0 )
	{

	}

	DataStream::~DataStream()
	{

	}

	dword DataStream::GetPosition() const
	{
		return mPointer;
	}

	void DataStream::Reset( )
	{
		mDataSize = 0;
		mPointer = 0;
	}

	bool DataStream::Seek( int offset, eMemoerySeekOrigin mso  )
	{
		switch( mso )
		{
		case MSO_Begin:
			{
				if ( (dword)offset > mDataSize || offset < 0 )
					return false;

				mPointer = offset;
				break;
			}
		case MSO_Current:
			{
				dword final = mPointer + offset;

				if ( final > mDataSize || final < 0 )
					return false;

				mPointer = final;

				break;
			}
		case MSO_End:
			{
				dword final = mDataSize - offset;

				if ( final > mDataSize || final < 0 )
					return false;

				mPointer = final;
				break;
			}
		default:
			return false;
		}
		
		return true;
	}

	dword DataStream::GetRemainBytes( ) const
	{
		if ( mPointer < mDataSize )
			return mDataSize - mPointer;
		
		return 0;
	}

	bool DataStream::ReadBuffer( void* output, dword numberOfBytesToRead )
	{
		Assert( output != null );

		if ( output == null )
			return false;

		// 没有数据可读
		if ( GetRemainBytes( ) < numberOfBytesToRead )
			return false;

		const char* finalbuffer = GetBuffer();
		Assert( finalbuffer != null );

		::memcpy( (char*)output, finalbuffer + mPointer, numberOfBytesToRead );

		mPointer += numberOfBytesToRead;

		return true;
	}

	bool DataStream::WriteBuffer( const void* input, dword numberOfBytes )
	{
		Assert( input != null );

		if ( input == null )
			return false;

		if ( numberOfBytes == 0 )
			return true;

		char* buffer = AllocBuffer( numberOfBytes );
		Assert( buffer != null )

		if ( buffer == null )
			return false;

		::memcpy( buffer + mDataSize, input, numberOfBytes );

		SetLength( GetSize() + numberOfBytes );

		return true;
	}

	bool DataStream::ReadBuffer( DataStream* s, dword numberOfBytes, dword extendbufferalloc  )
	{
		Assert( s != null );

		if ( s == null )
			return false;

		s->AllocBuffer( numberOfBytes + extendbufferalloc);

		if (!ReadBuffer( s->GetBuffer(), numberOfBytes ) )
			return false;

		s->SetLength( numberOfBytes );

		return true;
	}



	bool DataStream::WriteBuffer( DataStream* s )
	{
		Assert( s != null );

		if ( s == null )
			return false;

		if ( s->GetRemainBytes( ) == 0 )
			return true;

		return WriteBuffer( s->GetBuffer() , s->GetRemainBytes()  );

	}

	bool DataStream::IgnoreBytes( dword numberOfBytes )
	{
		// 没有数据可读
		if ( GetRemainBytes() < numberOfBytes )
			return false;

		Seek( numberOfBytes, MSO_Current );

		return true;
	}

	dword DataStream::GetSize( ) const
	{
		return mDataSize;
	}


	template<typename T>
	bool DataStream::WriteString( const T* str, dword len )
	{
		if (!WriteTypes<word>( (word)len ))
			return false;

		if ( len != 0 )
		{
			if (!WriteBuffer( str, sizeof(T)* len ) )
				return false;
		}

		return true;
	}


	bool DataStream::WriteUNICODEString( const WString& str )
	{
		return WriteString<wchar>( str.c_str(), str.length() );
	}

	bool DataStream::WriteUNICODEString( const wchar* str )
	{
		return WriteString<wchar>( str, ::wcslen( str ) );
	}

	bool DataStream::WriteANSIString( const AString& str )
	{
		return WriteString<char>( str.c_str(), str.length() );
	}

	bool DataStream::WriteANSIString( const char* str )
	{
		return WriteString<char>( str, ::strlen( str ) );
	}


	template<typename T, typename T2>
	bool DataStream::ReadString( T& Str )
	{

		word len = 0;
		if ( !ReadTypes<word>( len ) )
			return false;
		
		Str.resize(len);

		return ReadBuffer( (void*)Str.data(), len * sizeof(T2) );
	}

	AString DataStream::ReadANSIString( )
	{
		AString Str;
		ReadString<AString, char>( Str );

		return Str;
	}

	WString DataStream::ReadUNICODEString( )
	{
		WString Str;
		ReadString<WString, wchar>( Str );

		return Str;
	}

	bool DataStream::IgnoreUNICODEString( )
	{
		word len = 0;
		if ( ReadTypes<word>( len ) )
			false;

		return IgnoreBytes( len * sizeof(wchar) );
	}

	bool DataStream::IgnoreANSIString( )
	{
		word len = 0;
		if ( ReadTypes<word>( len ) )
			false;

		return IgnoreBytes( len * sizeof(char) );
	}

	char* DataStream::AllocBuffer( dword size )
	{
		return null;
	}

	void DataStream::ReleaseBuffer( )
	{
		mAllocedSize = 0;
	}

	bool DataStream::IsValid()
	{
		return true;
	}


	void DataStream::SetLength( dword len )
	{
		mDataSize = len;
	}
}