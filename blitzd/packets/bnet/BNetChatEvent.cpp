#include "Config.h"
#include "BNetChatEvent.h"
#include "core/Client.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetChatEvent::Pack()
		{
			_packet << eventid << uflag << ping << (uint)0 << (uint)0xBAADF00D << (uint)0xBAADF00D << username << text;
			return true;
		}

		void BNetChatEvent::SendToSet( std::set<Core::Client *>& uset, Core::Client * cl, bool ignoreSelf )
		{
			std::set<Core::Client *>::iterator it;
			for(it = uset.begin(); it != uset.end(); ++ it)
			{
				if(ignoreSelf && *it == cl)
					continue;
				(*it)->Send(_packet, _packet.size());
			}
		}
	}
}
