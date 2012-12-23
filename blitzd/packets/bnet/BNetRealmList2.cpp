#include "Config.h"
#include "BNetRealmList2.h"
#include "cache/RealmCache.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetRealmList2::Pack()
		{
			_packet << (uint)0;
			Cache::realmCache.BuildPacket(_packet);
			return true;
		}
	}
}
