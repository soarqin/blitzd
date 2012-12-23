#ifndef _BNETPROFILE_H_
#define _BNETPROFILE_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetProfile, 0x35FF)
			uint token;
			byte result;
			std::string description;
			std::string location;
			uint clan;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETPROFILE_H_
