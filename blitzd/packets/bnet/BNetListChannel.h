#ifndef _BNETLISTCHANNEL_H_
#define _BNETLISTCHANNEL_H_

#include "PacketBNet.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetListChannel, 0x0BFF)
			Core::Client * cl;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETLISTCHANNEL_H_
