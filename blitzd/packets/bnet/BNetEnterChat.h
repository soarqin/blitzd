#ifndef _BNETENTERCHAT_H_
#define _BNETENTERCHAT_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetEnterChat, 0x0AFF)
			std::string uname;
			std::string stats;
			std::string aname;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETENTERCHAT_H_
