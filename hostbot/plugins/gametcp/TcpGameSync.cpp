#include "Config.h"
#include "TcpGameSync.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpGameSync::Process( Network::TcpClient& client, Utils::Stream& st)
		{
			HostBot::Player& player = (HostBot::Player&)client;
			player.PushSync();
			return true;
		}
	}
}
