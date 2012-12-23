#include "Config.h"
#include "BaalCheckReply.h"

namespace Packets
{
	namespace Baal
	{
		bool BaalCheckReply::Pack()
		{
			_packet << result;
			return true;
		}
	}
}
