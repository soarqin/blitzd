#ifndef _BNETREADUSERDATA_H_
#define _BNETREADUSERDATA_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetReadUserData, 0x26FF)
			uint acount, kcount, token;
			std::vector<std::vector<std::string> > result;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETREADUSERDATA_H_
