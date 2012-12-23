#include "Config.h"
#include "SysTime.h"

namespace Utils
{
	static uint StartTime = GetTime();
	static uint startTick = GetTicks();
	uint GetTime()
	{
		return (uint)time(NULL);
	}
	uint GetTicks()
	{
#ifdef WIN32
		return GetTickCount( );
#else
		uint32_t ticks;
		struct timespec t;
		clock_gettime( CLOCK_MONOTONIC, &t );
		ticks = t.tv_sec * 1000;
		ticks += t.tv_nsec / 1000000;
		return ticks;
#endif
	}
	uint GetUpTime()
	{
		return GetTime() - StartTime;
	}

	uint GetUpTicks()
	{
		return GetTicks() - startTick;
	}
}
