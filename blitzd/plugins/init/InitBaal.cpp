#include "Config.h"
#include "InitBaal.h"
#include "core/Client.h"
#include "packets/baal/BaalAuthInfo.h"

namespace Plugins
{
	namespace Init
	{
		bool InitBaal::Process( Network::TcpClient& cl, Utils::Stream& )
		{
			LOG_DEBUG(("Got BaalD connection."));
			Core::Client& client = (Core::Client&)cl;
			client.SetStage(Core::Client::BAAL);

			Packets::Baal::BaalAuthInfo packet(0);
			packet.session = client.GetSession();
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
