#include "Config.h"
#include "BNetAccountChange.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAccountChange::Pack()
		{
			_packet << status;
			_packet.append(salt, 32);
			_packet.append(pubkey, 32);
			return true;
		}
	}
}
