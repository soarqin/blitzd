#include "Config.h"
#include "NetGamePort.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool NetGamePort::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			ushort port;
			in >> port;
			client.SetGamePort(port);
			return true;
		}
	}
}
