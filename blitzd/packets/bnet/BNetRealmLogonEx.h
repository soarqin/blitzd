#ifndef _BNETREALMLOGONEX_H_
#define _BNETREALMLOGONEX_H_

#include "PacketBNet.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetRealmLogonEx, 0x3EFF)
			uint cookie;
			std::string realmName;
			Core::Client * client;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETREALMLOGONEX_H_
