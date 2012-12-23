#ifndef _BNETPING_H_
#define _BNETPING_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetPing, 0x25FF)
			uint sid;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETPING_H_
