#include "Config.h"
#include "BNetOldAccountCreate2.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetOldAccountCreate2::Pack()
		{
			_packet << result;
			return true;
		}
	}
}
