#include "Config.h"
#include "BNetAdvListEx.h"
#include "cache/GameCache.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAdvListEx::Pack()
		{
			if(gamename.empty())
			{
				uint pos = _packet.size();
				_packet << (uint)0;
				uint c = 0;
				Cache::GameCache::GameMap_t& gamemap = Cache::gameCache.GetGames();
				Cache::GameCache::GameMap_t::iterator it;
				for(it = gamemap.begin(); it != gamemap.end(); ++ it)
				{
					Cache::GameCacheItem * item = it->second.get();
					if((mask == 0 || (item->GetType() & mask) == (cond & mask)) && item->GetBroadcast() && (item->GetHost() != NULL))
					{
						++ c;
						if(c > startidx && c <= maxcount)
						{
							_packet << item->GetType() << item->GetOption() << (ushort)2 << htons(item->GetHost()->GetGamePort()) << htonl(item->GetHost()->GetGameIP());
							_packet << (uint64)0 << ((item->GetFlag() & 0x02) | 0x04) << item->GetElapsed() << item->GetName().c_str() << item->GetPasswd().c_str() << item->GetStat().c_str();
						}
					}
					if(c == maxcount)
						break;
				}
				if(c == 0)
					_packet << (uint)0;
				*(uint *)&(_packet[pos]) = c;
			}
			else
			{
				Cache::GameCacheItem * item = Cache::gameCache[gamename];
				if((item == NULL) ||  (item->GetHost() == NULL))
				{
					_packet << (uint)0 << (uint)1;
				}
				else if((item->GetFlag() & 0x02) > 0)
				{
					_packet << (uint)0 << (uint)3;
				}
				else if((item->GetFlag() & 0x08) > 0)
				{
					_packet << (uint)0 << (uint)4;
				}
				else if(!item->GetPasswd().empty() && item->GetPasswd().c_str() != gamepass.c_str())
				{
					_packet << (uint)0 << (uint)2;
				}
				else
				{
					_packet << item->GetType() << item->GetOption() << (ushort)2 << htons(item->GetHost()->GetGamePort()) << htonl(item->GetHost()->GetGameIP());
					_packet << (uint64)0 << ((item->GetFlag() & 0x02) | 0x04) << item->GetElapsed() << item->GetName().c_str() << item->GetPasswd().c_str() << item->GetStat().c_str();
				}
			}
			return true;
		}
	}
}
