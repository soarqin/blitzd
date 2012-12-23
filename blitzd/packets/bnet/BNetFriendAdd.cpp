#include "Config.h"
#include "BNetFriendAdd.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetFriendAdd::Pack()
		{
			if(fr->user.get() == NULL)
				return false;
			std::string uname = fr->user->GetUsername();
			_packet << uname.c_str();
			Core::Client * clptr;
			if(cl == NULL)
			{
				clptr = Core::clientPool[uname];
			}
			else
			{
				clptr = cl;
			}
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
			return true;
		}
	}
}
