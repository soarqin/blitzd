#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Singleton.h"

namespace Utils
{
	class Random:
		public Singleton<Random>
	{
	public:
		Random();
		uint Next32();
		ushort Next16();
		byte Next8();
	};
	uint GetRandom32();
	ushort GetRandom16();
	byte GetRandom8();
}

#endif // _RANDOM_H_

