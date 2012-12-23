#include "Config.h"
#include "PacketUDP.h"

namespace Packets
{
	namespace UDP
	{
		bool PacketUDP::Build()
		{
			_packet << GetCmd();
			return Pack();
		}
	}
}
