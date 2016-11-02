#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Platform
	*  @{
	*/
	

	/** 
		操作系统线程类
	*/
	class Thread : public KernelObject
	{
	public:
		enum _THREAD_PRIORITY
		{
			_THREAD_PRIORITY_LOW	= 1,
			_THREAD_PRIORITY_NORMAL	= 2,
			_THREAD_PRIORITY_HIGH	= 3,
		};

		//! Define thread update callback function prototype.
		//! @param		parameter1		User defined parameter.
		//! @param		parameter2		User defined parameter.
		//! @return		True indicates success, false indicates this function is useless, must be remove from update functions list.
		typedef bool (*OnUpdateThread)( void* parameter1, void* parameter2 );
		//! Define thread close callback function prototype.
		//! @param		parameter1		User defined parameter.
		//! @param		parameter2		User defined parameter.
		typedef void (*OnCloseThread)( void* parameter1, void* parameter2 );

	private:
		//! Update function structure.
		struct UpdateFuncStruct
		{
			//! Update function pointer.
			OnUpdateThread	mUpdateThreadFunc;
			//! Update function parameter1.
			void*			mUpdateParameter1;
			//! Update function parameter2.
			void*			mUpdateParameter2;
		};

		//! APC function call structure.
		struct APCCallStruct
		{
			//! Update function pointer.
			OnUpdateThread	mUpdateThreadFunc;
			//! Update function parameter1.
			void*			mUpdateParameter1;
			//! Update function parameter2.
			void*			mUpdateParameter2;
			//! Finished event.
			EventObject*			mFinishedEvent;
		};

		//! Define update function array.
		typedef std::vector< UpdateFuncStruct > UpdateFuncArray;

		//! Update interval
		dword				mInterval;
		//! Thread ID
		dword				mThreadID;

		//! Wether self delete the thread object after close callback function called.
		bool				mSelfDelete;

		//! The critical section used internal.
		LockObject				mThreadLock;
		//! Thread ready event
		EventObject				mReadyEvent;
		//! Thread finished event.
		EventObject				mFinishedEvent;

		//! Update functions list.
		UpdateFuncArray		mUpdateFuncArray;

		//! Close function pointer.
		OnCloseThread		mCloseThreadFunc;
		//! Close function parameter1.
		void*				mCloseParameter1;
		//! Close function parameter2.
		void*				mCloseParameter2;

		//! The thread work function, called by win32 api.
		//! @param		parameter		User defined parameter.
		//! @return		The exit code of the thread.
		static long __stdcall ThreadProc( void* parameter );
		//! The APC function, used to execute a function in the thread or wake a sleeping thread up.
		//! @param		parameter		User defined parameter.
		static void __stdcall APCProc( void* parameter );

	public:
		//! Constructor, create a empty thread, that is not running.
		//! @param		none
		Thread( );
		//! Destructor, close thread and delete its kernel object.
		//! @param		none
		~Thread( );

		static bool CallFunctionUsingBackgroundThread( OnUpdateThread updatefuncpointer, OnCloseThread closefuncpointer, void* parameter1, void* parameter2 );

		//! Get thread ID.
		//! @param		none
		//! @return		The thread ID.
		inline dword GetThreadID( ) const;

		static dword GetCurrentThreadID( );

		//! Creates a thread to execute update functions.
		//! @param		interval		The sleep interval of thread in milliseconds
		//! @param		suspend			If it is true, thread is created in a suspended state and does not run until Resume is called.
		//! @return		True indicates success, false indicates failure.
		bool Create( dword interval, bool suspend = false, const char* threadname = 0 );
		//! Close the thread in a safe way.
		//! @param		none
		void Close( );
		//! Suspend the thread, ( suspend count is incremented ).
		//! @param		none
		//! @return		True indicates success, false indicates failure.
		bool Suspend( );
		//! Decrements suspend count. When it is decremented to zero, the thread is resumed.
		//! @param		none
		//! @return		True indicates success, false indicates failure.
		bool Resume( );
		//! Wake up the thread if it is sleeping.
		//! @param		none
		//! @return		True indicates success, false indicates failure.
		bool Wakeup( );
		//! Execute a function in the thread.
		//! @param		funcpointer		Update function pointer.
		//! @param		parameter1		User defined parameter.
		//! @param		parameter2		User defined parameter.
		bool CallFunction( OnUpdateThread funcpointer, void* parameter1, void* parameter2 );

		//! Set the sleep interval of thread in milliseconds, the thread will execute update functions every interval.
		//! @param		interval		The sleep interval of thread in milliseconds, the default interval is 100ms.
		void SetInterval( dword interval );
		//! Wait for thread end.
		//! @param		none
		void WaitForThreadEnd( );

		//! Enable or disable thread object self delete after close callback function called.
		//! @param		enable			Wether self delete the thread object.
		void EnableSelfDelete( bool enable );

		//! Set the priority level of thread.
		//! @param		priority		The priority to set.
		//! @return		True indicates success, false indicates failure.
		bool SetPriority( dword priority );

		//! Register an update function to thread, the function will be called every interval in the thread.
		//! @param		funcpointer		Update function pointer.
		//! @param		parameter1		User defined parameter.
		//! @param		parameter2		User defined parameter.
		void RegisterUpdateFunc( OnUpdateThread funcpointer, void* parameter1, void* parameter2 );
		//! Register a close function to thread, the function will be called when thread is being finished.
		//! @param		funcpointer		Close function pointer.
		//! @param		parameter1		User defined parameter.
		//! @param		parameter2		User defined parameter.
		void RegisterCloseFunc( OnCloseThread funcpointer, void* parameter1, void* parameter2 );
	};

	//----------------------------------------------------------------------------
	// Thread Implementation
	//----------------------------------------------------------------------------

	dword Thread::GetThreadID( ) const
	{
		return mThreadID;
	}

	/** @} */
	/** @} */
};