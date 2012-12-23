#ifndef _BNETACCOUNTCHANGE_H_
#define _BNETACCOUNTCHANGE_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAccountChange, 0x55FF)
			uint status;
			byte salt[32];
			byte pubkey[32];
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETACCOUNTCHANGE_H_
