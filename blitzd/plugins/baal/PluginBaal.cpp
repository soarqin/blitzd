#include "Config.h"
#include "PluginBaal.h"
#include "RealmAuth.h"
#include "UserCheck.h"

namespace Plugins
{
	namespace Baal
	{
		PLUGIN_SUITE_BEGIN(PluginBaal)
			PLUGIN_ADD(RealmAuth)
			PLUGIN_ADD(UserCheck)
		PLUGIN_SUITE_END()
	}
}
