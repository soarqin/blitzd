#include "Config.h"
#include "BroadcastServer.h"
#include "plugins/gameudp/PluginGameUdp.h"
#include "utils/Stream.h"

namespace HostBot
{
	void BroadcastServer::OnRecv( uint ip, ushort port, const byte * buf, size_t len )
	{
		if(len >= 4 && *(ushort *)(buf + 2) <= len)
		{
		    Utils::Stream st(buf + 4, len - 4);
			Plugins::GameUdp::PluginGameUdp::GetSingleton().Process(GetSingleton(), *(ushort *)&buf[0], ip, port, st);
		}
	}

	void BroadcastServer::Send( uint ip, ushort port, ushort op, const byte * buf, size_t len )
	{
		std::vector<byte> rbuf;
		GeneratePacket(op, buf, len, rbuf);
		Network::UdpSocket::Send(ip, port, rbuf);
	}

	void BroadcastServer::Broadcast( ushort port, ushort op, const byte * buf, size_t len )
	{
		std::vector<byte> rbuf;
		GeneratePacket(op, buf, len, rbuf);
		Network::UdpSocket::Broadcast(port, rbuf);
	}

	void BroadcastServer::GeneratePacket( ushort op, const byte * buf, size_t len, std::vector<byte>& rbuf )
	{
		rbuf.resize(4 + len);
		*(ushort *)&rbuf[0] = op;
		*(ushort *)&rbuf[2] = 4 + (ushort)len;
		if(len > 0)
			memcpy(&rbuf[4], buf, len);
	}
}
