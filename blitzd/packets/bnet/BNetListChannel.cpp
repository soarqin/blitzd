#include "Config.h"
#include "BNetListChannel.h"
#include "cache/ChannelCache.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetListChannel::Pack()
		{
			std::list<Cache::ChannelCacheItem::Pointer>::iterator it;
			std::list<Cache::ChannelCacheItem::Pointer>& itemlist = Cache::channelCache[cl->GetGameID()];
			for(it = itemlist.begin(); it != itemlist.end(); ++ it)
			{
				if(!(*it)->IsSpawned() && *it != cl->GetChannel())
				{
					_packet << (*it)->GetName();
				}
			}
			itemlist = Cache::channelCache[0];
			for(it = itemlist.begin(); it != itemlist.end(); ++ it)
			{
				if(!(*it)->IsSpawned() && *it != cl->GetChannel())
				{
					_packet << (*it)->GetName();
				}
			}
			_packet << (byte)0;
			return true;
		}
	}
}
