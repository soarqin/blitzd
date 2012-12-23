#include "Config.h"
#include "PluginGameUdp.h"
#include "UdpSearchGame.h"

namespace Plugins
{
	namespace GameUdp
	{
		PLUGIN_SUITE_BEGIN(PluginGameUdp)
			PLUGIN_ADD(UdpSearchGame)
		PLUGIN_SUITE_END()
	}
}
