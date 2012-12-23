#include "Config.h"
#include "BotLoop.h"

#include "GameServer.h"
#include "BroadcastServer.h"
#include "MapCfg.h"
#include "BotCfg.h"
#include "BotGameSet.h"
#include "utils/SysTime.h"
#include "utils/Thread.h"

namespace HostBot
{
	class GameTimer:
		public Network::SocketServer::ITimer
	{
	public:
		virtual bool operator()()
		{
			BotGameSet::GetSingleton().TimerCheck(Utils::GetTicks());
			return true;
		}
	};

	class PlayerTimer:
		public Network::SocketServer::ITimer
	{
	public:
		virtual bool operator()()
		{
			PlayerPool::GetSingleton().TimerCheck(Utils::GetTicks());
			return true;
		}
	};

	void BotLoop::Run()
	{
		botCfg.Load("hostbot.xml");
		LOG_DEBUG(("Loaded xml"));
		GameServer::GetSingleton().Init(botCfg.TcpPort());
		BroadcastServer::GetSingleton().Init(botCfg.UdpPort(), &GameServer::GetSingleton());
		LOG_DEBUG(("Network initialized"));

		static GameTimer gametimer;
		GameServer::GetSingleton().RegisterTimer(100, &gametimer);

		static PlayerTimer playertimer;
		GameServer::GetSingleton().RegisterTimer(1000, &playertimer);

		GameServer::GetSingleton().Run();

		while(1)
		{
			Utils::Thread::sleep(500);
		}
	}
}
