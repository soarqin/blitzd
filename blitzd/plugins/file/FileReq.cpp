#include "Config.h"
#include "FileReq.h"
#include "packets/file/FileHeader.h"

namespace Plugins
{
	namespace File
	{
		bool FileReq::Process( Network::TcpClient& cl, Utils::Stream& st )
		{
			uint plat, prod;
			Packets::File::FileHeader packet;
			uint64 ft;
			st >> plat >> prod >> packet.adid >> packet.extag >> packet.offset >> ft >> packet.fileName;
			LOG_DEBUG(("Requesting file %s...", packet.fileName.c_str()));
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
