#include "Config.h"
#include "BNetOldAuthLogon2.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetOldAuthLogon2::Pack()
		{
			_packet << result;
			if(!reason.empty())
				_packet << reason.c_str();
			return true;
		}
	}
}
