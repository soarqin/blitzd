#include "Config.h"
#include "TcpGameAction.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpGameAction::Process( Network::TcpClient& client, Utils::Stream& st)
		{
			if(st.size() <= 4)
				return true;
			HostBot::Player& player = (HostBot::Player&)client;
			player.PushAction(((byte *)st) + 4, st.size() - 4);
			return true;
		}
	}
}
