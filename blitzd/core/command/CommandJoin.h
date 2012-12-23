#ifndef _COMMANDJOIN_H_
#define _COMMANDJOIN_H_

#include "cache/ChannelCache.h"

COMMAND_HANDLER_BEGIN(CommandJoin, true)
	Cache::ChannelCacheItem::Pointer ptr = Cache::channelCache.Ensure(arg1, client.GetGameID(), client.GetCountryABR());
	if(ptr.get() != NULL)
	{
		ptr->Join(&client);
		return;
	}
	Core::Message::Send(&client, NULL, Core::Message::Info, "The channel is full.");
COMMAND_HANDLER_END()

#endif // _COMMANDJOIN_H_
