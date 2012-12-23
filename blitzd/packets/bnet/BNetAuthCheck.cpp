#include "Config.h"
#include "BNetAuthCheck.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetAuthCheck::Pack()
		{
			_packet << code << errorMsg.c_str();
			return true;
		}
	}
}
