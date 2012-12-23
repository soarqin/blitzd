#ifndef _BNETOLDAUTHLOGON2_H_
#define _BNETOLDAUTHLOGON2_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetOldAuthLogon2, 0x3AFF)
			uint result;
			std::string reason;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETOLDAUTHLOGON2_H_
