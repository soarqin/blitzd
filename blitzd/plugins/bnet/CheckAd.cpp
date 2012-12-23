#include "Config.h"
#include "CheckAd.h"

namespace Plugins
{
	namespace BNet
	{
		bool CheckAd::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			return true;
		}
	}
}
