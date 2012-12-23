#include "Config.h"
#include "TcpJoinGame.h"
#include "core/BotGameSet.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpJoinGame::Process( Network::TcpClient& client, Utils::Stream& in )
		{
			uint gameId;
			uint mapId;
			std::string userName;
			byte newSlotId; // for saved game only
			sockaddr_in addr;
			in>>gameId;
			in += 7;
			in>>mapId>>userName;
			in += 1;
			in>>newSlotId>>addr;

			HostBot::BotGame::Pointer game = HostBot::BotGameSet::GetSingleton()[gameId];
			if(game.get() == 0 || game->GetStage() != HostBot::BotGame::STAGE_PREPARE)
				return false;

			HostBot::Player& player = (HostBot::Player&)client;
			sockaddr_in inaddr = {0};
			player.SetAddr(inaddr);
			player.SetName(userName);
			if(!game->ApplyNewSlot(&player, newSlotId))
			{
				Utils::Stream st;
				st<<(uint)9;
				player.Send(0x05F7, st);
				player.PendClose();
				return true;
			}
			socklen_t len = sizeof(sockaddr_in);
			getpeername((int)client, (sockaddr *)&inaddr, &len);
			player.SetHost(game.get());
			HostBot::PlayerPool::GetSingleton().Add(&player);
			Utils::Stream st;
			game->GetSlotInfo(st);
			st<<(byte)player.GetPID()<<inaddr;
			player.Send(0x04F7, st);
			game->SendPlayersInfo(player);
			st.clear();
			game->BuildMapCheck(st);
			player.Send(0x3DF7, st);
			game->SendSlotInfo();
			return true;
		}
	}
}
