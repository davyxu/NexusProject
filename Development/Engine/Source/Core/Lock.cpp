#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	//----------------------------------------------------------------------------
	// Lock Implementation
	//----------------------------------------------------------------------------

	LockObject::LockObject( ) : mEnable( true )
	{
		::InitializeCriticalSection( (CRITICAL_SECTION*) this );
	}

	LockObject::~LockObject( )
	{
		::DeleteCriticalSection( (CRITICAL_SECTION*) this );
	}

	void LockObject::Enable( bool enable )
	{
		mEnable = enable;
	}

	void LockObject::Enter( ) const
	{
		if ( mEnable )
			::EnterCriticalSection( (CRITICAL_SECTION*) this );
	}

	void LockObject::Leave( ) const
	{
		if ( mEnable )
			::LeaveCriticalSection( (CRITICAL_SECTION*) this );
	}

	bool LockObject::TryEnter( ) const
	{
		if ( mEnable == false )
			return true;

		return ::TryEnterCriticalSection( (CRITICAL_SECTION*) this ) != 0;
	}

	int LockObject::IncrementLock( volatile int* Value)
	{
		return (int)InterlockedIncrement((LPLONG)Value);
	}

	int LockObject::DecrementLock( volatile int* Value)
	{
		return (int)InterlockedDecrement((LPLONG)Value);
	}




	//----------------------------------------------------------------------------
	// LockOwner Implementation
	//----------------------------------------------------------------------------

	LockOwner::LockOwner( const LockObject& lock ) : mLock( lock )
	{
		mLock.Enter( );
	}

	LockOwner::~LockOwner( )
	{
		mLock.Leave( );
	}
}
