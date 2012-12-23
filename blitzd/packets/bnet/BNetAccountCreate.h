#ifndef _BNETACCOUNTCREATE_H_
#define _BNETACCOUNTCREATE_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAccountCreate, 0x52FF)
			uint result;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETACCOUNTCREATE_H_
