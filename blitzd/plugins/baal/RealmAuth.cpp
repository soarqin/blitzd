#include "Config.h"
#include "RealmAuth.h"
#include "core/Client.h"
#include "cache/RealmCache.h"
#include "packets/baal/BaalAuthReply.h"

namespace Plugins
{
	namespace Baal
	{
		bool RealmAuth::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			uint token, session;
			ushort port;
			std::string name, desc;
			in >> token >> session >> port >> name >> desc;
			Packets::Baal::BaalAuthReply packet(token);
			if(session == client.GetSession())
			{
				Cache::RealmCacheItem::Pointer realm = Cache::realmCache[name];
				if(realm.get() == NULL)
				{
					sockaddr_in addr;
					socklen_t len = sizeof(sockaddr_in);
					if(getpeername((int)cl, (sockaddr *)&addr, &len) != SOCKET_ERROR)
					{
						Cache::realmCache.Add(name, desc, port, ntohl(addr.sin_addr.s_addr));
						LOG_DEBUG(("BaalD authed from %08X", addr.sin_addr.s_addr));
						packet.result = 0;
						packet.BuildAndSendTo(cl);
						return true;
					}
				}
			}
			packet.result = 1;
			packet.BuildAndSendTo(cl);
			cl.PendClose();
			return true;
		}
	}
}
