#include "Config.h"
#include "AuthInfo.h"
#include "packets/baal/BaalRealmAuth.h"

namespace Plugins
{
	namespace Baal
	{
		bool AuthInfo::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			uint token;
			in >> token;
			Packets::Baal::BaalRealmAuth packet(token);
			in >> packet.session;
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
