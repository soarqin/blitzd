#include "Config.h"
#include "BNetOldAccountChange.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetOldAccountChange::Pack()
		{
			_packet << result;
			return true;
		}
	}
}
