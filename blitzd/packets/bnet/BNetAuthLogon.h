#ifndef _BNETAUTHLOGON_H_
#define _BNETAUTHLOGON_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAuthLogon, 0x53FF)
			uint status;
			byte salt[32];
			byte pubkey[32];
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETAUTHLOGON_H_
