#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_

#include "IPlugin.h"
#include "utils/Singleton.h"
#include "utils/Stream.h"
#include "network/SocketBase.h"
#include "network/UdpSocket.h"

namespace Interface
{
	class PluginManager
	{
	public:
		bool Process(Network::TcpClient& client, ushort op, Utils::Stream& stream);
		bool Process(Network::UdpSocket& udp, ushort op, uint ip, ushort port, Utils::Stream& stream);
		void Load(IPlugin::Pointer ptr);
	private:
		IPlugin::Pointer _plugins[65536];
	};
}

#endif // _PLUGINMANAGER_H_
