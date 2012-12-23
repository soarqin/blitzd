#include "Config.h"
#include "ResetPassword.h"

namespace Plugins
{
	namespace BNet
	{
		bool ResetPassword::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			return true;
		}
	}
}
