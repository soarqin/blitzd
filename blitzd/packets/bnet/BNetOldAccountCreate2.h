#ifndef _BNETOLDACCOUNTCREATE2_H_
#define _BNETOLDACCOUNTCREATE2_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetOldAccountCreate2, 0x3DFF)
			uint result;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETOLDACCOUNTCREATE2_H_
