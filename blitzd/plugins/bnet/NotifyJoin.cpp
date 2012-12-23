#include "Config.h"
#include "NotifyJoin.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool NotifyJoin::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			uint gameid, verid;
			std::string name, pwd;
			in >> gameid >> verid >> name >> pwd;

			Cache::GameCacheItem::Pointer ptr = Cache::gameCache[name];
			if(ptr.get() != NULL)
			{
				ptr->Join(&client);
			}
			return true;
		}
	}
}
