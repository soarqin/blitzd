#ifndef _BNETREPLYNEWS_H_
#define _BNETREPLYNEWS_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetReplyNews, 0x46FF)
			uint last_time;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETREPLYNEWS_H_

