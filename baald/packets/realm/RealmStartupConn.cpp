#include "Config.h"
#include "RealmStartupConn.h"
#include "core/Cfg.h"

namespace Packets
{
	namespace Realm
	{
		bool RealmStartupConn::Pack()
		{
			_packet << result;
			return true;
		}
	}
}
