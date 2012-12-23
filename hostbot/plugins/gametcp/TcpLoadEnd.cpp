#include "Config.h"
#include "TcpLoadEnd.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpLoadEnd::Process( Network::TcpClient& client, Utils::Stream& )
		{
			HostBot::Player& player = (HostBot::Player&)client;
			player.LoadEnd();
			return true;
		}
	}
}
