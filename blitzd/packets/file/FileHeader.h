#ifndef _FILEHEADER_H_
#define _FILEHEADER_H_

#include "../tcp/PacketTCP.h"

namespace Packets
{
	namespace File
	{
		DECLARE_PACKET_TCP_BEGIN(FileHeader)
			std::string fileName;
			uint adid;
			uint extag;
			uint offset;
		DECLARE_PACKET_TCP_END()
	}
}


#endif // _FILEHEADER_H_
