#ifndef _FILETOKEN_H_
#define _FILETOKEN_H_

#include "../tcp/PacketTCP.h"

namespace Packets
{
	namespace File
	{
		DECLARE_PACKET_TCP_BEGIN(FileToken)
			uint token;
		DECLARE_PACKET_TCP_END()
	}
}

#endif // _FILETOKEN_H_

