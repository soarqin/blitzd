#include "Config.h"
#include "BaalUserCheck.h"

namespace Packets
{
	namespace Baal
	{
		bool BaalUserCheck::Pack()
		{
			_packet << cookie << hash << username;
			return true;
		}
	}
}
