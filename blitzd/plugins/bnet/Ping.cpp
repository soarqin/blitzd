#include "Config.h"
#include "Ping.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool Ping::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;

			uint sid;
			in >> sid;

			client.CheckEnd(sid);
			return true;
		}
	}
}
