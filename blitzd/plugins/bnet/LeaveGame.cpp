#include "Config.h"
#include "LeaveGame.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool LeaveGame::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			if(client.GetGame() != NULL)
			{
				client.GetGame()->Leave(&client);
			}
			return true;
		}
	}
}
