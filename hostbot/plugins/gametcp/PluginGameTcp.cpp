#include "Config.h"
#include "PluginGameTcp.h"
#include "TcpJoinGame.h"
#include "TcpLeaveGame.h"
#include "TcpMapSize.h"
#include "TcpChat.h"
#include "TcpLoadEnd.h"
#include "TcpGameAction.h"
#include "TcpGameSync.h"
#include "TcpPong.h"

namespace Plugins
{
	namespace GameTcp
	{
		PLUGIN_SUITE_BEGIN(PluginGameTcp)
			PLUGIN_ADD(TcpJoinGame)
			PLUGIN_ADD(TcpLeaveGame)
			PLUGIN_ADD(TcpMapSize)
			PLUGIN_ADD(TcpChat)
			PLUGIN_ADD(TcpLoadEnd)
			PLUGIN_ADD(TcpGameAction)
			PLUGIN_ADD(TcpGameSync)
			PLUGIN_ADD(TcpPong)
		PLUGIN_SUITE_END()
	}
}
