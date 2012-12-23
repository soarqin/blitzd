#include "Config.h"
#include "UDPResponse.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool UDPResponse::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			client.DelFlag(UF_NOUDP);
			return true;
		}
	}
}
