#include "Config.h"
#include "TcpMapSize.h"
#include "core/BotGame.h"
#include "core/Player.h"
#include "core/MapCfg.h"
#include "crypto/Crc32.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpMapSize::Process( Network::TcpClient& client, Utils::Stream& in )
		{
			byte fromid;
			uint mapid, completed;
			in>>mapid>>fromid>>completed;

			HostBot::Player& player = (HostBot::Player&)client;
			if(!player.SetMapProgress(completed) && completed == 0 && player.GetHost() != NULL)
			{
				Utils::Stream st2;
				st2<<mapid<<(byte)1;
				player.Send(0x3FF7, st2);

				byte pid = player.GetPID();
				HostBot::MapCfg::Pointer cfg = player.GetHost()->GetMapCfg();
				std::vector<uint>& crclist = cfg->GetCrcList();
				for(size_t idx = 0, pos = 0; pos < cfg->GetSize(); pos += 1442, ++ idx)
				{
					Utils::Stream st;
					st<<(byte)pid<<(byte)1<<mapid<<pos;
					uint sz = 1442;
					if(pos + sz > cfg->GetSize())
						sz = cfg->GetSize() - pos;
					st<<crclist[idx];
					st.append(cfg->GetData() + pos, sz);

					player.Send(0x43F7, st);
				}
			}
			return true;
		}
	}
}
