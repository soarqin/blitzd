#ifndef _BNETADVLISTEX_H_
#define _BNETADVLISTEX_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAdvListEx, 0x09FF)
			std::string gamename, gamepass, gamestat;
			uint startidx, maxcount, cond, mask;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETADVLISTEX_H_
