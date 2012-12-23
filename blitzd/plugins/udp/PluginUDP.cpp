#include "Config.h"
#include "PluginUDP.h"
#include "ConnTest2.h"

namespace Plugins
{
	namespace UDP
	{
		PLUGIN_SUITE_BEGIN(PluginUDP)
			PLUGIN_ADD(ConnTest2)
		PLUGIN_SUITE_END()
	}
}
