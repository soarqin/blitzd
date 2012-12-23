#include "Config.h"
#include "UDPServerPing.h"

namespace Packets
{
	namespace UDP
	{
		bool UDPServerPing::Pack()
		{
			_packet << (uint)'bnet';
			return true;
		}
	}
}
