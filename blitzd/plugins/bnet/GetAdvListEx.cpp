#include "Config.h"
#include "GetAdvListEx.h"
#include "packets/bnet/BNetAdvListEx.h"

namespace Plugins
{
	namespace BNet
	{
		bool GetAdvListEx::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetAdvListEx packet;
			in >> packet.cond >> packet.mask >> packet.startidx >> packet.maxcount >> packet.gamename >> packet.gamepass >> packet.gamestat;
			LOG_TRACE(("cond: 0x%08X mask: 0x%08X max count: %d name: %s pass: %s stat: %s", packet.cond, packet.mask, packet.maxcount, packet.gamename.c_str(), packet.gamepass.c_str(), packet.gamestat.c_str()));
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
