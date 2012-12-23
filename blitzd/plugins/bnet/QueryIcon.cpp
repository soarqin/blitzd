#include "Config.h"
#include "QueryIcon.h"
#include "core/Client.h"
#include "cache/FileCache.h"
#include "packets/bnet/BNetReplyIcon.h"

namespace Plugins
{
	namespace BNet
	{
		bool QueryIcon::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			std::string filename;
			if(client.GetProtocolVer() == 2)
				filename = "icons-WAR3.bni";
			else
				filename = "icons_STAR.bni";
			Cache::FileCacheItem::Pointer ptr = Cache::fileCache[filename];
			if(ptr.get() == NULL)
				return false;
			Packets::BNet::BNetReplyIcon packet;
			packet.filetime = ptr->GetFileTime();
			packet.filename = filename;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
