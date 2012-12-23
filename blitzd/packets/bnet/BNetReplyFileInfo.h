#ifndef _BNETREPLYFILEINFO_H_
#define _BNETREPLYFILEINFO_H_

#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		DECLARE_PACKET_BNET_BEGIN(BNetReplyFileInfo, 0x33FF)
			uint64 fileid;
			uint64 filetime;
			std::string filename;
		DECLARE_PACKET_BNET_END()
	}
}

#endif // _BNETREPLYFILEINFO_H_

