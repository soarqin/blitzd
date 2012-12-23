#ifndef _BNETFRIENDPOSITION_H_
#define _BNETFRIENDPOSITION_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetFriendPosition, 0x69FF)
			byte oindex;
			byte nindex;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETFRIENDPOSITION_H_
