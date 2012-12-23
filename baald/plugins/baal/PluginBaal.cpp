#include "Config.h"
#include "PluginBaal.h"
#include "AuthInfo.h"
#include "UserCheck.h"

namespace Plugins
{
	namespace Baal
	{
		PLUGIN_SUITE_BEGIN(PluginBaal)
			PLUGIN_ADD(AuthInfo)
			PLUGIN_ADD(UserCheck)
		PLUGIN_SUITE_END()
	}
}
