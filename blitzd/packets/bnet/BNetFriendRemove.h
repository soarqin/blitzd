#ifndef _BNETFRIENDREMOVE_H_
#define _BNETFRIENDREMOVE_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetFriendRemove, 0x68FF)
			byte index;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETFRIENDREMOVE_H_
