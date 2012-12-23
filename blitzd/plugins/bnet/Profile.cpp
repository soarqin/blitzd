#include "Config.h"
#include "Profile.h"
#include "packets/bnet/BNetProfile.h"
#include "cache/UserCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool Profile::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetProfile packet;
			std::string username;
			in >> packet.token >> username;
			Cache::UserCacheItem::Pointer ptr = Cache::userCache[username];
			if(ptr.get() == NULL)
			{
				return false;
			}
			packet.result = 0;
			packet.description = ptr->GetData("Profile\\Description");
			packet.location = ptr->GetData("Profile\\Location");
			packet.clan = 0; // TODO: clan support
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
