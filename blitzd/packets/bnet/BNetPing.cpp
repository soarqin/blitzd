#include "Config.h"
#include "BNetPing.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetPing::Pack()
		{
			_packet << sid;
			return true;
		}
	}
}
