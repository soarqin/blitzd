#include "Config.h"
#include "InitFile.h"
#include "core/Client.h"

namespace Plugins
{
	namespace Init
	{
		bool InitFile::Process( Network::TcpClient& cl, Utils::Stream& )
		{
			Core::Client& client = (Core::Client&)cl;
			client.SetStage(Core::Client::FILETRANS);
			return true;
		}
	}
}
