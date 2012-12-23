#include "Config.h"
#include "UdpSearchGame.h"
#include "core/BotGameSet.h"
#include "core/BotCfg.h"

bool Plugins::GameUdp::UdpSearchGame::Process( Network::UdpSocket&, uint ip, ushort port, Utils::Stream& in )
{
	uint platform, version;
	in>>platform>>version;
	if(platform != 'W3XP' || version != HostBot::botCfg.GameVersion())
		return false;
	HostBot::BotGameSet::GetSingleton().SendUDPInfo(ip, port);
	return true;
}
