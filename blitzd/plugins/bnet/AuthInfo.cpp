#include "Config.h"
#include "AuthInfo.h"
#include "core/Client.h"
#include "packets/bnet/BNetAuthInfo.h"

namespace Plugins
{
	namespace BNet
	{
		bool AuthInfo::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			uint protocol, platform, product, ver, gamelang, natIP, tzone, locale, bnlang;
			std::string country_abr, country_name;
			in >> protocol >> platform >> product >> ver >> gamelang >> natIP >> tzone >> locale >> bnlang;
			in >> country_abr >> country_name;
			client.SetAuthInfo(platform, product, ver, gamelang, natIP, tzone, locale, bnlang, country_abr, country_name);
			client.SendPing();
			Packets::BNet::BNetAuthInfo packet;
			packet.client = &client;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
