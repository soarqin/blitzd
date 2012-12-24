#include "Config.h"
#include "DaemonLoop.h"
#include "BNetTCPServer.h"
#include "BNetUDPServer.h"
#include "Cfg.h"
#include "cache/CheckRevision.h"
#include "cache/UserCache.h"
#include "cache/NewsCache.h"
#include "cache/ChannelCache.h"
#include "storage/Storage.h"

#include "games/war3/LadderInfo.h"

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

		cfg.Load("BlitzD.xml");

		Storage::Storage::SetEngine(cfg.GetDBEngine().c_str());
		Storage::storage->Open(cfg.GetDBFile().c_str());

		Cache::userCache.LoadFromDB();

		Cache::CheckRevision::GetSingleton().Load();

		Cache::newsCache.Load();

		Cache::channelCache.Load();

		Games::War3::LadderInfo::GetSingleton().Init();

		BNetTCPServer::GetSingleton().Init((ushort)cfg.GetTcpPort());
		BNetUDPServer::GetSingleton().Init((ushort)cfg.GetUdpPort(), &BNetTCPServer::GetSingleton());

		LOG_DEBUG(("Network initialized"));

		BNetTCPServer::GetSingleton().Run();

		_running = true;
		while(_running)
		{
			Utils::Thread::sleep(500);
		}
		BNetTCPServer::GetSingleton().Destroy();

		Cache::userCache.SaveToDB();

		Storage::storage->Optimize();
	}

	void DaemonLoop::Stop()
	{
		_running = false;
	}
}
