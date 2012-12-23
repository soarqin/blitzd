#ifndef _BAALAUTHINFO_H_
#define _BAALAUTHINFO_H_

#include "PacketBaal.h"

namespace Packets
{
	namespace Baal
	{
		DECLARE_PACKET_BAAL_BEGIN(BaalAuthInfo, 0x01)
			uint session;
		DECLARE_PACKET_BAAL_END()
	}
}

#endif // _BAALAUTHINFO_H_
