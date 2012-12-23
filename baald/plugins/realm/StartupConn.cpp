#include "Config.h"
#include "StartupConn.h"
#include "core/Client.h"
#include "core/BlitzConn.h"
#include "packets/baal/BaalUserCheck.h"

namespace Plugins
{
	namespace Realm
	{
		bool StartupConn::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			Packets::Baal::BaalUserCheck packet(client.GetSession());
			in >> packet.cookie;
			in += 40;
			in >> packet.hash >> packet.username;
			packet.BuildAndSendTo(Core::BlitzConn::Get());
			return true;
		}
	}
}
