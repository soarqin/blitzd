#include "Config.h"
#include "BNetProfile.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetProfile::Pack()
		{
			_packet << token << result;
			if(result == 0)
			{
				_packet << description << location << clan;
			}
			return true;
		}
	}
}
