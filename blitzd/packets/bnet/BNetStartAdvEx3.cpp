#include "Config.h"
#include "BNetStartAdvEx3.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetStartAdvEx3::Pack()
		{
			_packet << status;
			return true;
		}
	}
}
