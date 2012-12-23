#ifndef _BROADCASTSERVER_H_
#define _BROADCASTSERVER_H_

#include "network/UdpSocket.h"
#include "utils/Singleton.h"

namespace HostBot
{
	class BroadcastServer:
		public Utils::Singleton<BroadcastServer>,
		public Network::UdpSocket
	{
	public:
		void Send(uint ip, ushort port, ushort op, const byte * buf, size_t len);
		void Broadcast(ushort port, ushort op, const byte * buf, size_t len);
	protected:
		virtual void OnRecv(uint ip, ushort port, const byte * buf, size_t len);
		void GeneratePacket(ushort op, const byte * buf, size_t len, std::vector<byte>& rbuf);
	};
}

#endif // _BROADCASTSERVER_H_
