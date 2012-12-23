#include "Config.h"
#include "QueryRealms2.h"
#include "packets/bnet/BNetRealmList2.h"

namespace Plugins
{
	namespace BNet
	{
		bool QueryRealms2::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetRealmList2 packet;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
