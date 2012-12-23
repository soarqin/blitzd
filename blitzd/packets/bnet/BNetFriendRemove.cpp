#include "Config.h"
#include "BNetFriendRemove.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetFriendRemove::Pack()
		{
			_packet << index;
			return true;
		}
	}
}
