#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	//----------------------------------------------------------------------------
	// SyncObject Implementation
	//----------------------------------------------------------------------------

	bool SyncObject::Wait( dword milliseconds )
	{
		return ::WaitForSingleObject( mObjectHandle, milliseconds ) == WAIT_OBJECT_0;
	}

	//----------------------------------------------------------------------------
	// Event Implementation
	//----------------------------------------------------------------------------

	bool EventObject::Create( bool manualreset, bool initialstate, const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::CreateEvent( null, manualreset, initialstate, objectname );

		return mObjectHandle != null;
	}

	bool EventObject::Open( const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::OpenEvent( EVENT_ALL_ACCESS, 0, objectname );

		return mObjectHandle != null;
	}

	bool EventObject::Set( )
	{
		return ::SetEvent( mObjectHandle ) != 0;
	}

	bool EventObject::Reset( )
	{
		return ::ResetEvent( mObjectHandle ) != 0;
	}

	bool EventObject::Pulse( )
	{
		return ::PulseEvent( mObjectHandle ) != 0;
	}

	//----------------------------------------------------------------------------
	// Mutex Implementation
	//----------------------------------------------------------------------------

	bool MutexObject::Create( bool initialowner, const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::CreateMutex( null, initialowner, objectname );

		return mObjectHandle != null;
	}

	bool MutexObject::Open( const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::OpenMutex( MUTEX_ALL_ACCESS, 0, objectname );

		return mObjectHandle != null;
	}

	bool MutexObject::Release( )
	{
		return ::ReleaseMutex( mObjectHandle ) != 0;
	}

	//----------------------------------------------------------------------------
	// Semaphore Implementation
	//----------------------------------------------------------------------------

	bool SemaphoreObject::Create( dword initialcount, dword maximumcount, const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::CreateSemaphore( null, initialcount, maximumcount, objectname );

		return mObjectHandle != null;
	}

	bool SemaphoreObject::Open( const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::OpenSemaphore( SEMAPHORE_ALL_ACCESS, 0, objectname );

		return mObjectHandle != null;
	}

	bool SemaphoreObject::Release( dword releasecount )
	{
		return ::ReleaseSemaphore( mObjectHandle, releasecount, null ) != 0;
	}

	//----------------------------------------------------------------------------
	// Timer Implementation
	//----------------------------------------------------------------------------

	bool TimerObject::Create( bool manualreset, const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::CreateWaitableTimer( null, manualreset, objectname );

		return mObjectHandle != null;
	}

	bool TimerObject::Open( const wchar* objectname )
	{
		Close( );

		mObjectHandle = ::OpenWaitableTimer( TIMER_ALL_ACCESS, 0, objectname );

		return mObjectHandle != null;
	}

	bool TimerObject::SetInterval( dword interval, dword period )
	{
		LARGE_INTEGER nsinterval;

		nsinterval.QuadPart = interval;
		// Convert ms to 100 ns
		nsinterval.QuadPart = 1000 * nsinterval.QuadPart;
		// Negative values indicate relative time.
		nsinterval.QuadPart = - nsinterval.QuadPart;

		return ::SetWaitableTimer( mObjectHandle, &nsinterval, period, null, null, 0 ) != 0;
	}

	bool TimerObject::Cancel( )
	{
		return ::CancelWaitableTimer( mObjectHandle ) != 0;
	}
}

