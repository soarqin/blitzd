#ifndef _BNETREQEMAIL_H_
#define _BNETREQEMAIL_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetReqEmail, 0x59FF)
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETREQEMAIL_H_
