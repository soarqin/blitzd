#include "Config.h"
#include "InitD2GS.h"
#include "core/Client.h"

namespace Plugins
{
	namespace Init
	{
		bool InitD2GS::Process( Network::TcpClient& cl, Utils::Stream& )
		{
			Core::Client& client = (Core::Client&)cl;
			client.SetStage(Core::Client::D2GS);
			return true;
		}
	}
}
