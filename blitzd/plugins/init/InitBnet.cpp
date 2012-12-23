#include "Config.h"
#include "InitBnet.h"
#include "core/Client.h"

namespace Plugins
{
	namespace Init
	{
		bool InitBnet::Process( Network::TcpClient& cl, Utils::Stream& )
		{
			Core::Client& client = (Core::Client&)cl;
			client.SetStage(Core::Client::BNET);
			return true;
		}
	}
}
