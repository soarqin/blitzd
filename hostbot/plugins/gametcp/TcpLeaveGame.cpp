#include "Config.h"
#include "TcpLeaveGame.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpLeaveGame::Process( Network::TcpClient& client, Utils::Stream& in )
		{
			uint reason;
			in>>reason;

			HostBot::Player& player = (HostBot::Player&)client;
			player.SetLeaveReason(7);
			player.LeaveGame();
			return true;
		}
	}
}
