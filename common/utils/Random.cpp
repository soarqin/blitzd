#include "Config.h"
#include "Random.h"
#include "SysTime.h"
#include <ctime>

namespace Utils
{
	uint GetRandom32()
	{
		return Random::GetSingleton().Next32();
	}

	ushort GetRandom16()
	{
		return Random::GetSingleton().Next16();
	}

	byte GetRandom8()
	{
		return Random::GetSingleton().Next8();
	}

	Random::Random()
	{
		srand(GetTicks());
	}

	uint Random::Next32()
	{
		return ((uint)rand() & 0xFFF) + (((uint)rand() & 0xFFF) << 12) + (((uint)rand() & 0xFF) << 24);
	}

	ushort Random::Next16()
	{
		return ((ushort)rand() & 0xFFF) + (((ushort)rand() & 0xF) << 12);
	}

	byte Random::Next8()
	{
		return (byte)(rand() & 0xFF);
	}
}

