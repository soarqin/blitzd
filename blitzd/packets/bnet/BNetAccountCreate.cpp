#include "Config.h"
#include "BNetAccountCreate.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAccountCreate::Pack()
		{
			_packet << result;
			return true;
		}
	}
}
