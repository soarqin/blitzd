#ifndef _BNETFRIENDUPDATE_H_
#define _BNETFRIENDUPDATE_H_

#include "PacketBNet.h"
#include "cache/UserCache.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetFriendUpdate, 0x66FF)
			byte index;
			Core::Client * cl;
			Cache::UserCacheItem::Friend_t * fr;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETFRIENDUPDATE_H_
