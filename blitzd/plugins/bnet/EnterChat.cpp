#include "Config.h"
#include "EnterChat.h"
#include "core/Client.h"
#include "packets/bnet/BNetEnterChat.h"

namespace Plugins
{
	namespace BNet
	{
		bool EnterChat::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			Cache::UserCacheItem::Pointer user = client.GetUser();
			if(user.get() == 0)
				return false;
			std::string dispname;
			in >> dispname;

			Packets::BNet::BNetEnterChat packet;
			if(dispname.empty())
				packet.uname = user->GetUsername();
			else
				packet.uname = dispname;
			packet.stats = client.GetStats();
			packet.aname = user->GetUsername();
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
