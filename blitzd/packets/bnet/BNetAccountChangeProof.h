#ifndef _BNETACCOUNTCHANGEPROOF_H_
#define _BNETACCOUNTCHANGEPROOF_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAccountChangeProof, 0x56FF)
			uint status;
			byte proof[20];
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETACCOUNTCHANGEPROOF_H_
