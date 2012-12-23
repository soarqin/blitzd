#include "Config.h"
#include "PluginManager.h"

namespace Interface
{
	void PluginManager::Load( IPlugin::Pointer ptr )
	{
		ushort op = ptr->GetOP();
		if(op < 65536)
			_plugins[op] = ptr;
	}

	bool PluginManager::Process( Network::TcpClient& client, ushort op, Utils::Stream& stream )
	{
		IPlugin * plugin = _plugins[op].get();
		if(plugin == NULL)
		{
			LOG_INFO(("Received unknown op code: %04X", op));
			return false;
		}
		return plugin->Process(client, stream);
	}

	bool PluginManager::Process( Network::UdpSocket& udp, ushort op, uint ip, ushort port, Utils::Stream& stream )
	{
		IPlugin * plugin = _plugins[op].get();
		if(plugin == NULL)
			return false;
		return plugin->Process(udp, ip, port, stream);
	}
}
