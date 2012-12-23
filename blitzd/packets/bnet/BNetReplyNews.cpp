#include "Config.h"
#include "BNetReplyNews.h"
#include "cache/NewsCache.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetReplyNews::Pack()
		{
			std::map<uint, std::string> news;
			byte c = Cache::newsCache.GetAfter(last_time, news);
			_packet << (byte)(c + 1) << (uint)time(NULL) << Cache::newsCache.OldestTime() << Cache::newsCache.NewestTime();
			_packet << (uint)0 << Cache::newsCache.GetMotd().c_str();
			if(c > 0)
			{
				std::map<uint, std::string>::iterator it;
				for(it = news.begin(); it != news.end(); ++ it)
				{
					_packet << it->first << it->second.c_str();
				}
			}
			return true;
		}
	}
}
