#include "Config.h"
#include "BNetReplyIcon.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetReplyIcon::Pack()
		{
			_packet << filetime << filename;
			return true;
		}
	}
}
