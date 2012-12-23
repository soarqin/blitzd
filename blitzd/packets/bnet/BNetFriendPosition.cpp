#include "Config.h"
#include "BNetFriendPosition.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetFriendPosition::Pack()
		{
			_packet << oindex << nindex;
			return true;
		}
	}
}
