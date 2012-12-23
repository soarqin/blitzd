#include "Config.h"
#include "BNetAuthInfo.h"
#include "cache/FileCache.h"
#include "cache/CheckRevision.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAuthInfo::Pack()
		{
			std::string fn = "ver-ix86-";
			char tailstr[] = "0.mpq";
			tailstr[0] += (client->GetToken() % 8);
			fn = fn + tailstr;
			Cache::FileCacheItem::Pointer ptr = Cache::fileCache[fn];
			if(ptr.get() == NULL)
				return false;
			_packet << client->GetProtocolVer() << client->GetToken() << client->GetSession() << ptr->GetFileTime() << fn.c_str() << Cache::CheckRevision::GetSingleton().GetFormula();
			if(client->GetProtocolVer() == 2)
			{
				_packet.append_zero(128);
			}
			return true;
		}
	}
}
