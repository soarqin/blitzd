#include "Config.h"
#include "SwitchGameID.h"
#include "core/Client.h"

namespace Plugins
{
	namespace BNet
	{
		bool SwitchGameID::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			uint gameid;
			in >> gameid;
			Core::Client& client = (Core::Client&)cl;
			client.SetGameID(gameid);
			return true;
		}
	}
}
