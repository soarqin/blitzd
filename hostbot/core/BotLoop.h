#ifndef _BOTLOOP_H_
#define _BOTLOOP_H_

#include "utils/Singleton.h"

namespace HostBot
{
	class BotLoop:
		public Utils::Singleton<BotLoop>
	{
	public:
		void Run();
	};
}

#endif // _BOTLOOP_H_

