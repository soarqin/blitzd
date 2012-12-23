#include "Config.h"
#include "BNetAuthLogonProof.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAuthLogonProof::Pack()
		{
			_packet << status;
			_packet.append(proof, 20);
			if(!msg.empty())
				_packet << msg.c_str();
			return true;
		}
	}
}
