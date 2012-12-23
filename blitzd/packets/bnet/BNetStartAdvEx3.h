#ifndef _BNETSTARTADVEX3_H_
#define _BNETSTARTADVEX3_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetStartAdvEx3, 0x1CFF)
			uint status;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETSTARTADVEX3_H_
