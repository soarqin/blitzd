#ifndef _BNETFRIENDADD_H_
#define _BNETFRIENDADD_H_

#include "PacketBNet.h"
#include "cache/UserCache.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetFriendAdd, 0x67FF)
			byte index;
			Core::Client * cl;
			Cache::UserCacheItem::Friend_t * fr;
		DECLARE_PACKET_BNET_END()
	}
}
#endif // _BNETFRIENDADD_H_
