#ifndef _BNETAUTHCHECK_H_
#define _BNETAUTHCHECK_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAuthCheck, 0x51FF)
			uint code;
			std::string errorMsg;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETAUTHCHECK_H_
