#include "EnginePCH.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{

	AsyncTaskThread GAsyncTaskThread;

	static bool AsyncTaskThreadProc( void* parameter1, void* parameter2 )
	{
		while ( GAsyncTaskThread.IsEnable() )
		{
			GAsyncTaskThread.mAsyncTaskEvent.Wait();

			volatile void* ReadPointer = NULL;
			dword NumReadBytes = 0;
			while( GAsyncTaskThread.mInQueue.begin_read( ReadPointer,NumReadBytes ) )
			{
				AsyncTask** Command = (AsyncTask**)ReadPointer;

				(*Command)->OnAsyncTaskProcess();

				GAsyncTaskThread.mInQueue.finish_read(sizeof(void*));
				GAsyncTaskThread.FinishLoading( (*Command) );
			}
			
		}
		
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// StreamingThread
	//////////////////////////////////////////////////////////////////////////

	#define STREAMING_COMMAND_BUFFER_SIZE	(4*1024)

	AsyncTaskThread::AsyncTaskThread()
		: mEnable( false )
		, mInQueue( STREAMING_COMMAND_BUFFER_SIZE )
		, mOutQueue( STREAMING_COMMAND_BUFFER_SIZE )
	{

	}

	void AsyncTaskThread::Start()
	{
		Assert( !IsEnable() );

		mEnable = true;
		mThread.RegisterUpdateFunc( AsyncTaskThreadProc, 0, 0 );
		mThread.Create( 0, false,"AsyncTaskThread");
		mAsyncTaskEvent.Create(false, false);
	}

	void AsyncTaskThread::Stop()
	{
		if ( !mEnable )
			return;

		Flush();

		mEnable = false;

		// ½áÊøÑ­»·
		mAsyncTaskEvent.Set();

		mThread.Close();
	}


	void AsyncTaskThread::Flush( )
	{
		mFlushFence.BeginFence();

		mFlushFence.Wait();
	}


	bool AsyncTaskThread::IsInAsyncTaskThread( )
	{
		return mThread.Valid() || Thread::GetCurrentThreadID() == mThread.GetThreadID();
	}

	void AsyncTaskThread::FinishLoading( AsyncTask* task )
	{
		ringbuffer::alloc_context alloc( mOutQueue, sizeof( task ) );
		memcpy(alloc.get_alloc(), &task, sizeof(task));
	}

	void AsyncTaskThread::NotifyFinishedTask()
	{
		volatile void* ReadPointer = NULL;
		dword NumReadBytes = 0;
		while( mOutQueue.begin_read( ReadPointer,NumReadBytes ) )
		{
			AsyncTask** Command = (AsyncTask**)ReadPointer;
			(*Command)->OnAsyncTaskFinished();
			mOutQueue.finish_read(sizeof(void*));
		}
	}

	bool AsyncTaskThread::IsEnable()
	{
		return mEnable;
	}




	void AsyncTaskThread::Add( AsyncTask* Task )
	{
		if ( !mEnable )
			return;

		ringbuffer::alloc_context alloc( mInQueue, sizeof( Task ) );
		memcpy(alloc.get_alloc(), &Task, sizeof(Task));

		mAsyncTaskEvent.Set();
	}


	//////////////////////////////////////////////////////////////////////////
	// WaitingFinishTask
	//////////////////////////////////////////////////////////////////////////
	AsyncTaskFence::AsyncTaskFence(bool AutoDeleteSelf)
		: mNumFence(  0 )
		, mAutoDeleteSelf( AutoDeleteSelf )
	{

	}

	void AsyncTaskFence::BeginFence()
	{	
		LockObject::IncrementLock( (int*)&mNumFence );
		GAsyncTaskThread.Add( this );
	}

	void AsyncTaskFence::Wait()
	{
		while(mNumFence > 0)
		{
			GAsyncTaskThread.NotifyFinishedTask();

			// Yield CPU time while waiting.
			Application::Sleep( 0 );

			GAsyncTaskThread.mAsyncTaskEvent.Set( );
		};
	}

	void AsyncTaskFence::OnAsyncTaskProcess()
	{
		LockObject::DecrementLock( (int*)&mNumFence );
	}

	void AsyncTaskFence::OnAsyncTaskFinished()
	{
		
	}

}