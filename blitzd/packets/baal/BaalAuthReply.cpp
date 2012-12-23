#include "Config.h"
#include "BaalAuthReply.h"

namespace Packets
{
	namespace Baal
	{
		bool BaalAuthReply::Pack()
		{
			_packet << result;
			return true;
		}
	}
}
