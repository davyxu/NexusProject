#include "CorePCH.h"
#include <time.h>
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	LONGLONG		GTicksPerSec;		//每秒的频率
	LONGLONG		GLastElapsedTime;	//QPF使用的上次计数时间
	LARGE_INTEGER	GInitTime;
	LARGE_INTEGER	GAbsTime;
	float			GElapsedTime;

	//////////////////////////////////////////////////////////////////////////
	// TimeSource
	//////////////////////////////////////////////////////////////////////////
	void TimeSource::StaticInit( )
	{
		LARGE_INTEGER qwTicksPerSec;
		::QueryPerformanceFrequency( &qwTicksPerSec );
		GTicksPerSec = qwTicksPerSec.QuadPart;
		::QueryPerformanceCounter( &GInitTime );//记录绝对时间
		GLastElapsedTime = GInitTime.QuadPart;
		GElapsedTime = 0.2f;
	}

	float TimeSource::GetElapsedTime( )
	{
		return GElapsedTime;
	}

	void TimeSource::Tick( )
	{
		LARGE_INTEGER qwTime;
		::QueryPerformanceCounter( &qwTime );
		GElapsedTime = 1000 * (float) ( qwTime.QuadPart - GLastElapsedTime ) / (float) GTicksPerSec;
		GLastElapsedTime = qwTime.QuadPart;

	}

	float TimeSource::GetAppTime( )
	{
		::QueryPerformanceCounter( &GAbsTime );
		return float((GAbsTime.QuadPart- GInitTime.QuadPart)/(float)GTicksPerSec*1000);
	}


	void TimeSource::GetLocalTime(TimeData& Data)
	{
		::GetLocalTime( (LPSYSTEMTIME)&Data );		
	}

	NekoEngine::dword TimeSource::GetTickTime()
	{
		return ::GetTickCount();
	}

	//////////////////////////////////////////////////////////////////////////
	// TimeRuler
	//////////////////////////////////////////////////////////////////////////
	TimeRuler::TimeRuler( )
	{
		Start();
	}

	float TimeRuler::GetCost()
	{
		return TimeSource::GetAppTime() - mStartTick;
	}

	void TimeRuler::Start()
	{
		mStartTick = TimeSource::GetAppTime();
	}

}
