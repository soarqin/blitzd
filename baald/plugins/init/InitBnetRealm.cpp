#include "Config.h"
#include "InitBnetRealm.h"
#include "core/Client.h"

namespace Plugins
{
	namespace Init
	{
		bool InitBnetRealm::Process( Network::TcpClient& cl, Utils::Stream& )
		{
			Core::Client& client = (Core::Client&)cl;
			client.SetStage(Core::Client::BNETREALM);
			return true;
		}
	}
}
