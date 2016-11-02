#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{
	#define RENDERING_COMMAND_BUFFER_SIZE	(256*1024)

	RenderThread GRenderThread;

	RenderThread::RenderThread()
		: mRenderCommandBuffer( RENDERING_COMMAND_BUFFER_SIZE )
		, mEnable( false )
	{

	}



	static bool RenderThreadProc( void* parameter1, void* parameter2 )
	{	
		while(GRenderThread.mEnable)
		{
			GRenderThread.mRenderEvent.Wait();

			// Execute pending render commands.
			volatile void* ReadPointer = NULL;
			dword NumReadBytes = 0;
			while(GRenderThread.mRenderCommandBuffer.begin_read(ReadPointer,NumReadBytes))
			{
				RenderCommand* Command = (RenderCommand*)ReadPointer;
				dword CommandSize = Command->Execute();
				Command->~RenderCommand();
				GRenderThread.mRenderCommandBuffer.finish_read(CommandSize);
			}
	
		};		

		return true;
	}

	void RenderThread::Start( )
	{
		Assert( !mEnable );

		mEnable = true;
		mRenderThread.RegisterUpdateFunc( RenderThreadProc, 0, 0 );
		
		mRenderEvent.Create(false, false );

		mRenderThread.Create( 0, false,"RenderThread");		
	}

	void RenderThread::NotifyNewTask( )
	{
		mRenderEvent.Set();
	}

	void RenderThread::Stop( )
	{
		if ( !mEnable )
			return;

		Flush();

		mEnable = false;

		// 跑完循环
		mRenderEvent.Set();

		
		mRenderThread.Close();		
	}

	float RenderThread::Flush()
	{
		float tm = TimeSource::GetAppTime();

		RenderCommandFence Fence;

		// 插入一个等待事件
		Fence.BeginFence();

		// 等待计数器归零(被执行后)
		Fence.Wait();

		return TimeSource::GetAppTime() - tm;
	}

	void RenderCommandFence::BeginFence()
	{
		LockObject::IncrementLock( (int*)&NumPendingFences );

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			FenceCommand,
			RenderCommandFence*,Fence,this,
			{
				LockObject::DecrementLock( (int*)&Fence->NumPendingFences );
			});
	}


	/**
	 * Waits for pending fence commands to retire.
	 * @param NumFencesLeft	- Maximum number of fence commands to leave in queue
	 */
	void RenderCommandFence::Wait( dword NumFencesLeft/*=0*/ ) const
	{
		Assert(IsInGameThread());

		while(NumPendingFences > NumFencesLeft)
		{

			// Yield CPU time while waiting.
			Application::Sleep( 0 );

			GRenderThread.mRenderEvent.Set();
		};
	}



}