#include "Config.h"
#include "StopAdv.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool StopAdv::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			if(client.GetGame() != NULL && client.GetGame()->IsHost(&client))
				client.GetGame()->SetBroadcast(false);
			return true;
		}
	}
}
