#include "Config.h"
#include "DaemonLoop.h"
#include "ShalTCPServer.h"
#include "Cfg.h"

#include <signal.h>

namespace Core
{
	extern "C" void sigbreak(int sig)
	{
		DaemonLoop::GetSingleton().Stop();
	}

	void DaemonLoop::Run()
	{
#ifndef SIGBREAK
#define SIGBREAK 21
#endif
		signal(SIGABRT, &sigbreak);
		signal(SIGINT, &sigbreak);
		signal(SIGTERM, &sigbreak);
		signal(SIGBREAK, &sigbreak);

		cfg.Load("shald.xml");

		ShalTCPServer::GetSingleton().Init((ushort)cfg.GetTcpPort());
		LOG_DEBUG(("Network initialized"));

		ShalTCPServer::GetSingleton().Run();

		_running = true;
		while(_running)
		{
			Utils::Thread::sleep(500);
		}
		ShalTCPServer::GetSingleton().Destroy();
	}

	void DaemonLoop::Stop()
	{
		_running = false;
	}
}
