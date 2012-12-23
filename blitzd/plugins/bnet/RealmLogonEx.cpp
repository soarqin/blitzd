#include "Config.h"
#include "RealmLogonEx.h"
#include "packets/bnet/BNetRealmLogonEx.h"

namespace Plugins
{
	namespace BNet
	{
		bool RealmLogonEx::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetRealmLogonEx packet;
			in >> packet.cookie;
			in += 20;
			in >> packet.realmName;
			packet.client = &(Core::Client&)cl;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
