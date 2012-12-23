#include "Config.h"
#include "RawReq.h"
#include "packets/file/FileHeader.h"

namespace Plugins
{
	namespace File
	{
		bool RawReq::Process( Network::TcpClient& cl, Utils::Stream& st )
		{
			Packets::File::FileHeader packet;
			uint64 ft;
			st >> packet.offset >> ft;
			st += 40;
			st >> packet.fileName;
			packet.adid = 0;
			packet.extag = 0;
			packet.BuildAndSendTo(cl);

			return true;
		}
	}
}
