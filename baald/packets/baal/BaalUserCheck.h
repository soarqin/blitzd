#ifndef _BAALUSERCHECK_H_
#define _BAALUSERCHECK_H_

#include "PacketBaal.h"

namespace Packets
{
	namespace Baal
	{
		DECLARE_PACKET_BAAL_BEGIN(BaalUserCheck, 0x10)
			uint cookie;
			byte hash[20];
			std::string username;
		DECLARE_PACKET_BAAL_END()
	}
}

#endif // _BAALUSERCHECK_H_
