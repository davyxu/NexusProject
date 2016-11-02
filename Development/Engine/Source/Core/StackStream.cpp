#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{
	StackStream::StackStream( )
	{
		mAllocedSize = DEFAULT_PREALLOC_SIZE;
	}

	StackStream::StackStream( const void* data, dword size	)
	{
		mAllocedSize = DEFAULT_PREALLOC_SIZE;
		WriteBuffer( data, size );
	}

	char* StackStream::GetBuffer() 
	{
		return mStackData;
	}

	char* StackStream::AllocBuffer( dword size )
	{
		// 内存对齐分配,按照需求的2倍分配
		dword newalloc = mDataSize + size;//Math::Align(mDataSize + size, ALIGN_ALLOC_STEP );

		// 没到达需要分配时，使用原来已经分配过的空间
		if ( newalloc < mAllocedSize && mAllocedSize > 0)
		{
			return mStackData;
		}

		Assert( "Out of StackStream DEFAULT_PREALLOC_SIZE");

		return null;
	}

}