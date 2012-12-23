#include "Config.h"
#include "BNetAccountChangeProof.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAccountChangeProof::Pack()
		{
			_packet << status;
			_packet.append(proof, 20);
			return true;
		}
	}
}
