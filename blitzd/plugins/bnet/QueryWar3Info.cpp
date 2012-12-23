#include "Config.h"
#include "QueryWar3Info.h"
/*#include "core/Client.h"*/
#include "packets/bnet/BNetWar3Info.h"

namespace Plugins
{
	namespace BNet
	{
		bool QueryWar3Info::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			/*Core::Client& client = (Core::Client&)cl;*/
			byte subcmd;
			in >> subcmd;
			LOG_TRACE(("Sub command of war3 gaming: %u", subcmd));
			switch(subcmd)
			{
			case 2:
				{
					byte c;
					uint cookie;
					in >> cookie >> c;
					if(c == 0)
						return true;
					for(byte i = 0; i < c; i ++)
					{
						Packets::BNet::BNetWar3Info packet;
						packet.subcmd = 2;
						packet.cookie = cookie;
						uint unk;
						in >> packet.tag >> unk;
						LOG_TRACE(("tag %d: %08X", i, packet.tag));

						packet.is_last = (i == c - 1);
						packet.BuildAndSendTo(cl);
					}
				}
				break;
			default:
				break;
			}
			return true;
		}
	}
}
