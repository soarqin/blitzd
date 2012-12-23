#include "Config.h"
#include "LeaveChat.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool LeaveChat::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			if(client.GetChannel() != NULL)
				client.GetChannel()->Leave(&client);
			return true;
		}
	}
}
