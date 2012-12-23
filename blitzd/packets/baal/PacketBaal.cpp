#include "Config.h"
#include "PacketBaal.h"

namespace Packets
{
	namespace Baal
	{
		PacketBaal::PacketBaal( uint t ): _token(t)
		{
		}

		bool PacketBaal::Build()
		{
			_packet << (ushort)0 << GetCmd() << _token;
			if(!Pack())
				return false;
			*(ushort *)&_packet[0] = (ushort)_packet.size();
			return true;
		}
	}
}
