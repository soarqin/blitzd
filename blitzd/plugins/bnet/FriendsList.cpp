#include "Config.h"
#include "FriendsList.h"
#include "core/Client.h"
#include "packets/bnet/BNetFriendsList.h"

namespace Plugins
{
	namespace BNet
	{
		bool FriendsList::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			Packets::BNet::BNetFriendsList packet;
			packet.cl = &client;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
