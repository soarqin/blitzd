#include "Config.h"
#include "BaalAuthInfo.h"

namespace Packets
{
	namespace Baal
	{
		bool BaalAuthInfo::Pack()
		{
			_packet << session;
			return true;
		}
	}
}
