#include "Config.h"
#include "JoinChannel.h"
#include "core/Client.h"
#include "core/Message.h"
#include "cache/ChannelCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool JoinChannel::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			uint flag;
			std::string channelname;
			in >> flag >> channelname;
			LOG_TRACE(("Joining channel %s with flag %u", channelname.c_str(), flag));
			Cache::ChannelCacheItem::Pointer ptr = Cache::channelCache.Ensure(channelname, client.GetGameID(), client.GetCountryABR());
			if(ptr.get() != NULL)
			{
				ptr->Join(&client);
				return true;
			}
			Core::Message::Send(&client, NULL, Core::Message::Info, "The channel is full.");
			return true;
		}
	}
}
