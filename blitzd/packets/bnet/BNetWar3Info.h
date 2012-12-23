#ifndef _BNETWAR3INFO_H_
#define _BNETWAR3INFO_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetWar3Info, 0x44FF)
			byte subcmd;
			uint cookie;
			uint tag;
			bool is_last;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETWAR3INFO_H_
