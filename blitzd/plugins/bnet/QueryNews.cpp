#include "Config.h"
#include "QueryNews.h"
#include "packets/bnet/BNetReplyNews.h"

namespace Plugins
{
	namespace BNet
	{
		bool QueryNews::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetReplyNews packet;
			in >> packet.last_time;

			packet.BuildAndSendTo(cl);

			return true;
		}
	}
}
