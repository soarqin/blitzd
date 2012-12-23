#ifndef _BNetUDPSERVER_H_
#define _BNetUDPSERVER_H_

#include "network/UdpSocket.h"
#include "utils/Singleton.h"
#include "utils/Stream.h"

namespace Core
{
	class BNetUDPServer:
		public Utils::Singleton<BNetUDPServer>,
		public Network::UdpSocket
	{
	public:
		void Send(uint, ushort, Utils::Stream&);
	protected:
		virtual void OnRecv(uint ip, ushort port, const byte * buf, size_t len);
	};
}

#endif // _BNetUDPSERVER_H_
