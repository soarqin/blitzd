#include "Config.h"
#include "TcpChat.h"
#include "core/Player.h"

namespace Plugins
{
	namespace GameTcp
	{
		bool TcpChat::Process( Network::TcpClient& client, Utils::Stream& in )
		{
			byte count = 0, fromid, flag;
			std::vector<byte> toid;
			in>>count;
			toid.resize(count);
			in.read(&toid[0], count);
			in>>fromid>>flag;
			if(flag < 16 || (flag > 20 && flag != 32))
				return false;

			HostBot::Player& player = (HostBot::Player&)client;
			switch(flag)
			{
			case 16:
				{
					std::string chat;
					in>>chat;
					player.GlobalChat(toid, fromid, chat);
				}
				break;
			case 32:
				{
					uint flag;
					std::string chat;
					in>>flag>>chat;
					player.InGameChat(toid, fromid, flag, chat);
				}
				break;
			case 17:
				{
					byte team;
					in>>team;
					player.SetTeam(team);
				}
				break;
			case 18:
				{
					byte color;
					in>>color;
					player.SetColor(color);
				}
				break;
			case 19:
				{
					byte race;
					in>>race;
					player.SetRace(race);
				}
				break;
			case 20:
				{
					byte handicap;
					in>>handicap;
					player.SetHandicap(handicap);
				}
				break;
			}

			return true;
		}
	}
}
