#ifndef _BNETOLDACCOUNTCHANGE_H_
#define _BNETOLDACCOUNTCHANGE_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetOldAccountChange, 0x31FF)
			uint result;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETOLDACCOUNTCHANGE_H_
