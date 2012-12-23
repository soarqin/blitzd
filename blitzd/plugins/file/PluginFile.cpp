#include "Config.h"
#include "PluginFile.h"
#include "FileReq.h"
#include "TokenReq.h"
#include "RawReq.h"

namespace Plugins
{
	namespace File
	{
		PLUGIN_SUITE_BEGIN(PluginFile)
			PLUGIN_ADD(FileReq)
			PLUGIN_ADD(TokenReq)
			PLUGIN_ADD(RawReq)
		PLUGIN_SUITE_END()
	}
}
