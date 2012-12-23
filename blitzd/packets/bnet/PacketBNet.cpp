#include "Config.h"
#include "PacketBNet.h"

namespace Packets
{
	namespace BNet
	{
		bool PacketBNet::Build()
		{
			_packet << (uint)GetCmd();
			if(!Pack())
				return false;
			*(ushort *)&_packet[2] = (ushort)_packet.size();
			return true;
		}
	}
}
