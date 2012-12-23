#include "Config.h"
#include "Null.h"
#include "packets/bnet/BNetNull.h"

namespace Plugins
{
	namespace BNet
	{
		bool Null::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetNull packet;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
