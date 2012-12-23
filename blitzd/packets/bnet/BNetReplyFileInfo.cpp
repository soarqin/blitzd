#include "Config.h"
#include "BNetReplyFileInfo.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetReplyFileInfo::Pack()
		{
			_packet << fileid << filetime << filename;
			return true;
		}
	}
}
