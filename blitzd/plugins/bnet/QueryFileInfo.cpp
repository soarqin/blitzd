#include "Config.h"
#include "QueryFileInfo.h"
#include "cache/FileCache.h"
#include "packets/bnet/BNetReplyFileInfo.h"

namespace Plugins
{
	namespace BNet
	{
		bool QueryFileInfo::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetReplyFileInfo packet;
			in >> packet.fileid >> packet.filename;
			Cache::FileCacheItem::Pointer ptr = Cache::fileCache[packet.filename];
			if(ptr.get() == NULL)
				return false;
			packet.filetime = ptr->GetFileTime();
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
