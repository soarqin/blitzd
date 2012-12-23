#include "Config.h"
#include "PluginInit.h"
#include "InitBnetRealm.h"
#include "InitD2GS.h"

namespace Plugins
{
	namespace Init
	{
		PLUGIN_SUITE_BEGIN(PluginInit)
			PLUGIN_ADD(InitBnetRealm)
			PLUGIN_ADD(InitD2GS)
		PLUGIN_SUITE_END()
	}
}
