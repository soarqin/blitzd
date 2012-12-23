#ifndef _BAALREALMAUTH_H_
#define _BAALREALMAUTH_H_

#include "PacketBaal.h"

namespace Packets
{
	namespace Baal
	{
		DECLARE_PACKET_BAAL_BEGIN(BaalRealmAuth, 0x02)
			uint session;
		DECLARE_PACKET_BAAL_END()
	}
}

#endif // _BAALREALMAUTH_H_
