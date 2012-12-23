#include "Config.h"
#include "PluginInit.h"
#include "InitBnet.h"
#include "InitFile.h"
#include "InitBaal.h"

namespace Plugins
{
	namespace Init
	{
		PLUGIN_SUITE_BEGIN(PluginInit)
			PLUGIN_ADD(InitBnet)
			PLUGIN_ADD(InitFile)
			PLUGIN_ADD(InitBaal)
		PLUGIN_SUITE_END()
	}
}
