#ifndef _DAEMONLOOP_H_
#define _DAEMONLOOP_H_

#include "utils/Singleton.h"
#include "BlitzConn.h"

namespace Core
{
	class DaemonLoop:
		public Utils::Singleton<DaemonLoop>
	{
	public:
		void Run();
		void Stop();
	private:
		bool _running;
	};
}

#endif // _DAEMONLOOP_H_
