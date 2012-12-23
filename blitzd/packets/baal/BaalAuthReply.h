#ifndef _BAALAUTHREPLY_H_
#define _BAALAUTHREPLY_H_

#include "PacketBaal.h"

namespace Packets
{
	namespace Baal
	{
		DECLARE_PACKET_BAAL_BEGIN(BaalAuthReply, 0x02)
			uint result;
		DECLARE_PACKET_BAAL_END()
	}
}

#endif // _BAALAUTHREPLY_H_
