#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	//----------------------------------------------------------------------------
	// KernelObject Implementation
	//----------------------------------------------------------------------------

	KernelObject::KernelObject( ) : mObjectHandle( null )
	{
	}

	KernelObject::KernelObject( handle objecthandle ) : mObjectHandle( objecthandle )
	{
	}

	KernelObject::KernelObject( const KernelObject& kernelobject )
	{
		::DuplicateHandle( ::GetCurrentProcess( ), (handle) kernelobject, ::GetCurrentProcess( ),
			&mObjectHandle, 0, 0, DUPLICATE_SAME_ACCESS );
	}

	KernelObject::~KernelObject( )
	{
		Close( );
	}

	KernelObject& KernelObject::operator = ( const KernelObject& kernelobject )
	{
		Close( );

		::DuplicateHandle( ::GetCurrentProcess( ), (handle) kernelobject, ::GetCurrentProcess( ),
			&mObjectHandle, 0, 0, DUPLICATE_SAME_ACCESS );

		return *this;
	}

	void KernelObject::Close( )
	{
		if ( mObjectHandle != null && mObjectHandle != INVALID_HANDLE_VALUE )
			::CloseHandle( mObjectHandle );

		mObjectHandle = null;
	}

	handle KernelObject::CloneHandle( ) const
	{
		if ( mObjectHandle == null || mObjectHandle == INVALID_HANDLE_VALUE )
			return mObjectHandle;

		handle clonedhandle = null;

		::DuplicateHandle( ::GetCurrentProcess( ), mObjectHandle, ::GetCurrentProcess( ),
			&clonedhandle, 0, 0, DUPLICATE_SAME_ACCESS );

		return clonedhandle;
	}
}

