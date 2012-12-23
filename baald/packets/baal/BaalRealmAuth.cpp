#include "Config.h"
#include "BaalRealmAuth.h"
#include "core/Cfg.h"

namespace Packets
{
	namespace Baal
	{
		bool BaalRealmAuth::Pack()
		{
			_packet << session << (ushort)Core::cfg.GetTcpPort() << Core::cfg.GetName() << Core::cfg.GetDescription();
			return true;
		}
	}
}
