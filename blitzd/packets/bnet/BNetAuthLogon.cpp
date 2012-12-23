#include "Config.h"
#include "BNetAuthLogon.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAuthLogon::Pack()
		{
			_packet << status;
			_packet.append(salt, 32);
			_packet.append(pubkey, 32);
			return true;
		}
	}
}
