#ifndef _BNETAUTHINFO_H_
#define _BNETAUTHINFO_H_

#include "PacketBNet.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAuthInfo, 0x50FF)
			Core::Client * client;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETAUTHINFO_H_
