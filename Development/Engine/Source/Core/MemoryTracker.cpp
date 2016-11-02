#include "CorePCH.h"
#include "Core/Lock.h"

namespace NekoEngine
{
	MemoryTracker* GMemoryTracker = null;

	LockObject GMemoryLocker;

	MemoryTracker::MemoryTracker()
		: mEnableTrack( true )
	{

	}

	void* AllocM( dword size )
	{
		return ::HeapAlloc( ::GetProcessHeap(), 0, size );
	}

	void FreeM( void* m )
	{
		::HeapFree( ::GetProcessHeap(), 0, m );
	}


	void MemoryTracker::Check( bool freeleeakmem )
	{
		if ( mMemoryMap.size() == 0 )
			return;

		mLogger.Debug(L"============Memory Leak List Begin!============");
		for ( MemoryMap::iterator it = mMemoryMap.begin();
			it != mMemoryMap.end();
			++it )
		{
			AllocData* data = it;
			mLogger.Debug(L"%s(%d) size = %d ptr=0x%p", 
								data->mFileName,
								data->mLine,
								data->mSize,
								data->mData );

			if ( freeleeakmem )
			{
				FreeM( data );
			}
		}

		mLogger.Debug(L"Total %d memory leaks!", mMemoryMap.size() );

		mLogger.Debug(L"============Memory Leak List End============");

		if ( freeleeakmem )
			mMemoryMap.clear();
	}

	void* MemoryTracker::Alloc( dword size,const wchar* filename, dword line )
	{
		void* buffer = AllocM( size );

		AllocData* data = (AllocData*)AllocM( sizeof( AllocData ) );
		if ( data == null )
		{
			//dword e = ::GetLastError( );
			return null;
		}

		data->mData		= buffer;
		data->mSize		= size;
		data->mFileName = filename;
		data->mLine		= line;
		
		LockOwner locker( GMemoryLocker );
		mMemoryMap.insert( data->mData, data );

		return buffer;
	}

	void MemoryTracker::Free( void* p )
	{
		if ( p == null )
			return;

		AllocData* data;

		{
			LockOwner locker( GMemoryLocker );
			AllocData** dataptr = mMemoryMap.find( p );

			// 不是这个分配器分配的内存
			if ( dataptr == null )
			{
				Assert( "Memory is not alloced by MemoryTracker" == 0 );
				return;
			}

			data = *dataptr;

			mMemoryMap.remove( p );
		}
	

		FreeM( data->mData );
		FreeM( data );
	}
}