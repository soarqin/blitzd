#ifndef _BNETFRIENDSLIST_H_
#define _BNETFRIENDSLIST_H_

#include "PacketBNet.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetFriendsList, 0x65FF)
			Core::Client * cl;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETFRIENDSLIST_H_
