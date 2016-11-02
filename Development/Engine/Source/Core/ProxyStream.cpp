#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{
	ProxyStream::ProxyStream( )
		: mHost( null )
		, mMaxSize( 0 )
		, mInitPosition( 0 )
	{

	}

	ProxyStream::ProxyStream( DataStream* m, dword numberOfBytes  )
	{
		mHost = m;
		mMaxSize = numberOfBytes;
		mInitPosition = m->GetPosition();
	}


	bool ProxyStream::WriteBuffer( const void* input, dword numberOfBytes )
	{
		return mHost->WriteBuffer( input, numberOfBytes );
	}

	bool ProxyStream::ReadBuffer( void* output, dword numberOfBytesToRead )
	{
		if ( GetRemainBytes() < numberOfBytesToRead )
			return false;

		return mHost->ReadBuffer( output, numberOfBytesToRead );
	}

	char* ProxyStream::GetBuffer() 
	{
		return mHost->GetBuffer();
	}

	dword ProxyStream::GetRemainBytes() const
	{
		dword Position = GetPosition();

		if ( Position < mMaxSize )
			return mMaxSize - Position;

		return 0;
	}

	dword ProxyStream::GetPosition() const
	{
		return mHost->GetPosition() - mInitPosition;
	}

	bool ProxyStream::Seek( int offset, eMemoerySeekOrigin mso )
	{
		if ( (dword)offset > mMaxSize )
			return false;

		return mHost->Seek( mInitPosition + offset, MSO_Begin );
	}

	void ProxyStream::SetLength( dword offset )
	{
		return mHost->SetLength( offset );
	}

	void ProxyStream::Reset()
	{
		mHost->Seek( mInitPosition, MSO_Begin );
	}

	dword ProxyStream::GetSize() const
	{
		return mMaxSize;
	}


}