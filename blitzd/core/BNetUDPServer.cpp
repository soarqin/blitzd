#include "Config.h"
#include "BNetUDPServer.h"
#include "plugins/udp/PluginUDP.h"

namespace Core
{
	void BNetUDPServer::OnRecv( uint ip, ushort port, const byte * buf, size_t len )
	{
		if(len < 4)
			return;
		uint op = *(uint *)buf;
		Utils::Stream st(buf + 4, len - 4);
		Plugins::UDP::PluginUDP::GetSingleton().Process(*this, op, ip, port, st);
	}

	void BNetUDPServer::Send( uint ip, ushort port, Utils::Stream& out )
	{
		Network::UdpSocket::Send(ip, port, out);
	}
}
