#ifndef _BAALCHECKREPLY_H_
#define _BAALCHECKREPLY_H_

#include "PacketBaal.h"

namespace Packets
{
	namespace Baal
	{
		DECLARE_PACKET_BAAL_BEGIN(BaalCheckReply, 0x10)
			uint result;
		DECLARE_PACKET_BAAL_END()
	}
}

#endif // _BAALCHECKREPLY_H_
