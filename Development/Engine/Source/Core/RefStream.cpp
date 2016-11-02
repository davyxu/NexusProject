#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{
	RefStream::RefStream( )
		: mRefData( null )
	{

	}

	RefStream::RefStream( const void* data, dword size )
		: mRefData( null )
	{
		mRefData = (char*)data;
		mDataSize = size;
	}

	RefStream::RefStream( DataStream* m, dword numberOfBytes  )
	{
		mRefData = m->GetBuffer();
		mDataSize = numberOfBytes;
	}

	RefStream::RefStream( DataStream* m )
	{
		mRefData = m->GetBuffer();
		mDataSize = m->GetSize();
	}


	bool RefStream::WriteBuffer( const void* input, dword numberOfBytes )
	{
		Assert( "RefStream read only memory content!" == 0 );

		return true;
	}

	char* RefStream::GetBuffer() 
	{
		return (char*)mRefData;
	}

	char* RefStream::AllocBuffer( dword size )
	{
		return (char*)mRefData;
	}

}