#ifndef _BNETCHATEVENT_H_
#define _BNETCHATEVENT_H_

#include "PacketBNet.h"
#include "cache/ChannelCache.h"

namespace Cache
{
	class ChannelCacheItem;
}

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetChatEvent, 0x0FFF)
			uint eventid;
			uint uflag;
			uint ping;
			std::string username;
			std::string text;

			void SendToSet(std::set<Core::Client *>&, Core::Client *, bool);
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETCHATEVENT_H_
