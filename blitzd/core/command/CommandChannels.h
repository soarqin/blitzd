#ifndef _COMMANDCHANNELS_H_
#define _COMMANDCHANNELS_H_

#include "cache/ChannelCache.h"

COMMAND_HANDLER_BEGIN(CommandChannels, true)
	std::list<Cache::ChannelCacheItem::Pointer>& item = Cache::channelCache[client.GetGameID()];
	Message::Send(&client, NULL, Message::Info, "Channels:");
	std::list<Cache::ChannelCacheItem::Pointer>::iterator it;
	for(it = item.begin(); it != item.end(); ++ it)
	{
		Message::Send(&client, NULL, Message::Info, "  " + (*it)->GetName());
	}
COMMAND_HANDLER_END()

#endif // _COMMANDCHANNELS_H_
