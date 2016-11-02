#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{

	bool IsInRenderingThread()
	{
		if ( GRenderThread.mRenderThread.Valid() )
			return Thread::GetCurrentThreadID() == GRenderThread.mRenderThread.GetThreadID();

		return true;
	}

	bool IsInGameThread()
	{
		if ( GGameThreadID == 0 )
			return true;

		return Thread::GetCurrentThreadID() == GGameThreadID;
	}

	bool IsInAsyncTaskThread( )
	{
		return Thread::GetCurrentThreadID() == GAsyncTaskThread.mThread.GetThreadID();
	}

}