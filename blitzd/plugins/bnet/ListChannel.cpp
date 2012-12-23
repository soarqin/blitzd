#include "Config.h"
#include "ListChannel.h"
#include "packets/bnet/BNetListChannel.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool ListChannel::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetListChannel packet;
			packet.cl = (Core::Client *)&cl;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
