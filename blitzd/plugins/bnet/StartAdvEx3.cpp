#include "Config.h"
#include "StartAdvEx3.h"
#include "core/Client.h"
#include "packets/bnet/BNetStartAdvEx3.h"
#include "cache/GameCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool StartAdvEx3::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			uint flag, elapse, type, option, ladder;
			std::string gname, gpass, gstat;
			in >> flag >> elapse >> type >> option >> ladder >> gname >> gpass >> gstat;
			LOG_TRACE(("Got game: %s, pass: %s, flag: %08X, type: %08X", gname.c_str(), gpass.c_str(), flag, type));

			Packets::BNet::BNetStartAdvEx3 packet;
			Cache::GameCacheItem::Pointer ptr = Cache::gameCache[gname];
			if(ptr.get() != NULL)
			{
				if((ptr->GetFlag() & 0x0E) == 0)
					packet.status = 1;
				else
				{
					ptr->UpdateStatus(flag, elapse, type, option, ladder);
					ptr->SetPasswd(gpass);
					ptr->SetStat(gstat);
					packet.status = 0;
				}
			}
			else
			{
				ptr = Cache::gameCache.Add(gname);
				ptr->UpdateStatus(flag, elapse, type, option, ladder);
				ptr->SetPasswd(gpass);
				ptr->SetStat(gstat);
				ptr->Join(&client);
				packet.status = 0;
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
