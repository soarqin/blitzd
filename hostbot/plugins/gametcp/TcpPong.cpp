#include "Config.h"
#include "TcpPong.h"
#include "utils/SysTime.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpPong::Process( Network::TcpClient& client, Utils::Stream& in)
		{
			if(in.left() < 4)
				return false;
			uint tick;
			in>>tick;

			HostBot::Player& player = (HostBot::Player&)client;

			player.SetPing(Utils::GetTicks() - tick);
			return true;
		}
	}
}
