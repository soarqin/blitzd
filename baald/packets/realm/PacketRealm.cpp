#include "Config.h"
#include "PacketRealm.h"

namespace Packets
{
	namespace Realm
	{
		bool PacketRealm::Build()
		{
			_packet << (ushort)0 << (byte)GetCmd();
			if(!Pack())
				return false;
			*(ushort *)&_packet[0] = (ushort)_packet.size();
			return true;
		}
	}
}
