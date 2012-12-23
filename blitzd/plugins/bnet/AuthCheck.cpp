#include "Config.h"
#include "AuthCheck.h"
#include "core/Client.h"
#include "cache/CheckRevision.h"
#include "packets/bnet/BNetAuthCheck.h"

namespace Plugins
{
	namespace BNet
	{
		bool AuthCheck::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			uint ticks, exever, exechksum, count, spawn;
			std::string exeinfo, owner;
			in >> ticks >> exever >> exechksum >> count >> spawn;
			in += count * 36;
			in >> exeinfo >> owner;
			LOG_TRACE(("Logged in with checksum: 0x%08X, exever: 0x%08X", exechksum, exever));

			Cache::CheckRevision::Revision * rev = Cache::CheckRevision::GetSingleton().findRev(client.GetToken() % 8, exechksum, exever);
			Packets::BNet::BNetAuthCheck packet;
			if(rev == NULL || rev->gameId != client.GetGameID())
				packet.code = 0x101;
			else
				packet.code = 0;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
