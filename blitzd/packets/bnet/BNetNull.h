#ifndef _BNETNULL_H_
#define _BNETNULL_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetNull, 0x00FF)
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETNULL_H_
