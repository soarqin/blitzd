#include "Config.h"
#include "UserCheck.h"
#include "core/Client.h"
#include "packets/realm/RealmStartupConn.h"

namespace Plugins
{
	namespace Baal
	{
		bool UserCheck::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			uint token;
			in >> token;
			Core::Client * client = Core::clientPool[token];
			if(client == NULL)
				return false;
			Packets::Realm::RealmStartupConn packet;
			in >> packet.result;
			packet.BuildAndSendTo(*client);
			return true;
		}
	}
}
