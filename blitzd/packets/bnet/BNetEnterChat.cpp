#include "Config.h"
#include "BNetEnterChat.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetEnterChat::Pack()
		{
			_packet << uname.c_str() << stats.c_str() << aname.c_str();
			return true;
		}
	}
}
