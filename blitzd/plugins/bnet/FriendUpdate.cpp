#include "Config.h"
#include "FriendUpdate.h"
#include "core/Client.h"
#include "cache/UserCache.h"
#include "packets/bnet/BNetFriendUpdate.h"

namespace Plugins
{
	namespace BNet
	{
		bool FriendUpdate::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			byte index;
			in >> index;

			Cache::UserCacheItem::Pointer ptr = client.GetUser();
			if(ptr.get() == NULL || index >= ptr->GetFriendCount())
				return true;

			Packets::BNet::BNetFriendUpdate packet;
			packet.cl = NULL;
			packet.index = index;
			packet.fr = ptr->GetFriend(index);

			return true;
		}
	}
}
