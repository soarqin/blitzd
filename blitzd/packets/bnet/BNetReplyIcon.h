#ifndef _BNETREPLYICON_H_
#define _BNETREPLYICON_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetReplyIcon, 0x2DFF)
			uint64 filetime;
			std::string filename;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETREPLYICON_H_
