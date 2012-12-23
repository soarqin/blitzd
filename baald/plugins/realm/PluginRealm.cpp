#include "Config.h"
#include "PluginRealm.h"
#include "StartupConn.h"

namespace Plugins
{
	namespace Realm
	{
		PLUGIN_SUITE_BEGIN(PluginRealm)
			PLUGIN_ADD(StartupConn)
		PLUGIN_SUITE_END()
	}
}
