#ifndef _BNETAUTHLOGONPROOF_H_
#define _BNETAUTHLOGONPROOF_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetAuthLogonProof, 0x54FF)
			uint status;
			byte proof[20];
			std::string msg;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETAUTHLOGONPROOF_H_
