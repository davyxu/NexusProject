#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	//----------------------------------------------------------------------------
	// Thread Implementation
	//----------------------------------------------------------------------------

	Thread::Thread( ) : mInterval( 100 ), mThreadID( 0 ), mSelfDelete( false ), mCloseThreadFunc( null )
	{
	}

	Thread::~Thread( )
	{
		Close( );
	}

	long Thread::ThreadProc( void* parameter )
	{
		Thread* thread = (Thread*) parameter;

		thread->mReadyEvent.Set( );

		while ( thread->mInterval != -1 )
		{
			thread->mThreadLock.Enter( );

			// Call all registered update function.
			for ( UpdateFuncArray::iterator it = thread->mUpdateFuncArray.begin(); it != thread->mUpdateFuncArray.end(); it ++ )
			{
				const UpdateFuncStruct& updatefunc = *it;

				if ( updatefunc.mUpdateThreadFunc == null )
					continue;

				// Call the function by pointer and parameters, if it returns false, remove it from fucntion list.
				if ( (*updatefunc.mUpdateThreadFunc)( updatefunc.mUpdateParameter1, updatefunc.mUpdateParameter2 ) == false )
				{
					thread->mUpdateFuncArray.erase( it );

					it --;
				}
			}

			thread->mThreadLock.Leave( );

			if ( thread->mSelfDelete == true )
				break;

			// 在Close中强制关闭
			if ( thread->mInterval == -1 )
				break;

			dword sleeptime = thread->mInterval;

			if ( thread->mUpdateFuncArray.size( ) == 0 )
				sleeptime *= 10;

			// Use SleepEx to make sure can be woken up.
			::SleepEx( sleeptime, true );
		}

		OnCloseThread closefunc = thread->mCloseThreadFunc;
		void* parameter1 = thread->mCloseParameter1;
		void* parameter2 = thread->mCloseParameter2;

		thread->mFinishedEvent.Set( );

		if ( closefunc != null )
			(*closefunc)( parameter1, parameter2 );

		if ( thread->mSelfDelete == true )
			delete thread;

		return 0;
	}

	void Thread::APCProc( void* parameter )
	{
		APCCallStruct* apccall = (APCCallStruct*) parameter;
		if ( parameter == null )
			return;

		Assert( apccall->mUpdateThreadFunc != null )
			Assert( apccall->mFinishedEvent != null )

			// Call the function.
			(*apccall->mUpdateThreadFunc)( apccall->mUpdateParameter1, apccall->mUpdateParameter2 );

		// Set the event, tell the caller function it finished.
		apccall->mFinishedEvent->Set( );
	}

	bool Thread::CallFunctionUsingBackgroundThread( OnUpdateThread updatefuncpointer, OnCloseThread closefuncpointer, void* parameter1, void* parameter2 )
	{
		Thread* thread = new Thread;

		thread->EnableSelfDelete( true );
		thread->RegisterUpdateFunc( updatefuncpointer, parameter1, parameter2 );
		thread->RegisterCloseFunc( closefuncpointer, parameter1, parameter2 );

		if ( thread->Create( 100, true ) == false )
		{
			delete thread;
			return false;
		}

		if ( thread->SetPriority( _THREAD_PRIORITY_LOW ) == false )
		{
			delete thread;
			return false;
		}

		if ( thread->Resume( ) == false )
		{
			delete thread;
			return false;
		}

		return true;
	}

	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;		// must be 0x1000
		LPCSTR szName;		// pointer to name (in user addr space)
		DWORD dwThreadID;	// thread ID (-1=caller thread)
		DWORD dwFlags;		// reserved for future use, must be zero
	} THREADNAME_INFO;

	void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName )
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = szThreadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;

		__try
		{
			RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
		}
		__except(EXCEPTION_CONTINUE_EXECUTION)
		{
			int a =1;
		}
	}


	bool Thread::Create( dword interval, bool suspend, const char* threadname )
	{
		// Make sure dont create multiple threads.
		if ( mObjectHandle != null )
			return false;

		// Create the thread ready events.
		if ( mReadyEvent.Create( false, false ) == false )
		{ Close( ); return false; }

		// Create the thread finished events.
		if ( mFinishedEvent.Create( true, false ) == false )
		{ Close( ); return false; }

		// Create the thread.
		mObjectHandle = ::CreateThread( null, 0, (LPTHREAD_START_ROUTINE) ThreadProc, this, suspend ? CREATE_SUSPENDED : 0, &mThreadID );


		if ( mObjectHandle == null )
		{ 
			Close( );
			return false; 
		}
// 在挂接IDE调试中,设置线程名称可能在频繁的创建删除同名线程中造成在Sleep的死锁, 非调试环境无关系
#ifdef _DEBUG
		if ( threadname )
			SetThreadName( mThreadID, threadname );
#endif

		mInterval = interval;

		// Wait for thread to finish making itself.
		if ( suspend == false )
			mReadyEvent.Wait( );

		return true;
	}

	void Thread::Close( )
	{
		if ( mObjectHandle == null || mThreadID == 0 )
			return;

		mThreadID = 0;
		mInterval = -1;

		// Wait for thread to finish exiting itself.
		mFinishedEvent.Wait( );

		mReadyEvent.Close( );
		mFinishedEvent.Close( );

		KernelObject::Close( );

		mUpdateFuncArray.clear();
	}

	bool Thread::Suspend( )
	{
		return ::SuspendThread( mObjectHandle ) != -1;
	}

	bool Thread::Resume( )
	{
		return ::ResumeThread( mObjectHandle ) != -1;
	}

	bool Thread::Wakeup( )
	{
		return ::QueueUserAPC( (PAPCFUNC) APCProc, mObjectHandle, null ) != 0;
	}

	bool Thread::CallFunction( OnUpdateThread funcpointer, void* parameter1, void* parameter2 )
	{
		// If already in the current thread, just call it directly.
		if ( ::GetCurrentThreadId( ) == mThreadID )
		{
			(*funcpointer)( parameter1, parameter2 );

			return true;
		}

		// Set up the call struct.
		EventObject finishevent;

		if ( finishevent.Create( false, false ) == false )
			return false;

		APCCallStruct apccall;
		apccall.mUpdateThreadFunc = funcpointer;
		apccall.mUpdateParameter1 = parameter1;
		apccall.mUpdateParameter2 = parameter2;
		apccall.mFinishedEvent	  = &finishevent;

		// Call the function with APC
		if ( ::QueueUserAPC( (PAPCFUNC) APCProc, mObjectHandle, (dword) &apccall ) == 0 )
			return false;

		// Wait for completion.
		finishevent.Wait( );

		return true;
	}

	void Thread::SetInterval( dword interval )
	{
		mInterval = interval;
	}

	void Thread::WaitForThreadEnd( )
	{
		mFinishedEvent.Wait( );
	}

	void Thread::EnableSelfDelete( bool enable )
	{
		mSelfDelete = enable;
	}

	bool Thread::SetPriority( dword priority )
	{
		if ( priority == _THREAD_PRIORITY_HIGH )
			priority = THREAD_PRIORITY_ABOVE_NORMAL;
		else if ( priority == _THREAD_PRIORITY_NORMAL )
			priority = THREAD_PRIORITY_NORMAL;
		else if ( priority == _THREAD_PRIORITY_LOW )
			priority = THREAD_PRIORITY_BELOW_NORMAL;
		else
			return false;

		return ::SetThreadPriority( mObjectHandle, priority ) != 0 ;
	}

	void Thread::RegisterUpdateFunc( OnUpdateThread funcpointer, void* parameter1, void* parameter2 )
	{
		LockOwner lockowner( mThreadLock );

		UpdateFuncStruct updatefunc;
		updatefunc.mUpdateThreadFunc = funcpointer;
		updatefunc.mUpdateParameter1 = parameter1;
		updatefunc.mUpdateParameter2 = parameter2;

		mUpdateFuncArray.push_back( updatefunc );

		Wakeup( );
	}

	void Thread::RegisterCloseFunc( OnCloseThread funcpointer, void* parameter1, void* parameter2 )
	{
		LockOwner lockowner( mThreadLock );

		mCloseThreadFunc = funcpointer;
		mCloseParameter1 = parameter1;
		mCloseParameter2 = parameter2;
	}

	NekoEngine::dword Thread::GetCurrentThreadID()
	{
		return ::GetCurrentThreadId();
	}
}
