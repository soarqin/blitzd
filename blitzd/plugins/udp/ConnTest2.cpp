#include "Config.h"
#include "ConnTest2.h"
#include "core/Client.h"
#include "packets/udp/UDPServerPing.h"

namespace Plugins
{
	namespace UDP
	{
		bool ConnTest2::Process( Network::UdpSocket& cl, uint ip, ushort port, Utils::Stream& in )
		{
			if(in.left() < 8)
				return false;
			uint token;
			in >> token;
			Core::Client * client = Core::clientPool[token];
			client->SetGameIP(ip);
			client->SetGamePort(port);
			Packets::UDP::UDPServerPing packet;
			packet.BuildAndSendTo(cl, ip, port);
			return true;
		}
	}
}
