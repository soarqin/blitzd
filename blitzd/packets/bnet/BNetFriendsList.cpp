#include "Config.h"
#include "BNetFriendsList.h"
#include "cache/UserCache.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetFriendsList::Pack()
		{
			Cache::UserCacheItem::Pointer ptr = cl->GetUser();
			if(ptr.get() == NULL)
				return false;
			byte cc = (byte)ptr->GetFriendCount();
			_packet << cc;
			for(byte i = 0; i < cc; ++ i)
			{
				Cache::UserCacheItem::Friend_t * fr = ptr->GetFriend(i);
				if(fr->user.get() == NULL)
					continue;
				std::string uname = fr->user->GetUsername();
				_packet << uname.c_str();
				Core::Client * clptr = Core::clientPool[uname];
				byte status = 0;
				byte location = 0;
				uint gameid = 0;
				std::string locname;
				if(clptr != NULL)
				{
					status = (fr->mutual ? 1 : 0);
					gameid = clptr->GetGameID();
					if(clptr->GetGame() != NULL)
					{
						if(clptr->GetGame()->GetFlag() & 1)
						{
							if(fr->mutual)
							{
								location = 5;
								locname = clptr->GetGame()->GetName();
							}
							else
							{
								location = 4;
							}
						}
						else
						{
							location = 3;
							locname = clptr->GetGame()->GetName();
						}
					}
					else if(clptr->GetChannel())
					{
						location = 2;
						locname = clptr->GetChannel()->GetName();
					}
					else
					{
						location = 1;
					}
					if(!clptr->GetDND().empty())
						status |= 2;
					if(!clptr->GetAway().empty())
						status |= 4;
				}
				_packet << status << location << gameid << locname;
			}
			return true;
		}
	}
}
