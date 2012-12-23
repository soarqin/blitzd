#ifndef _BNetTCPSERVER_H_
#define _BNetTCPSERVER_H_

#include "utils/Singleton.h"
#include "network/SocketServer.h"

namespace Core
{
	class BNetTCPServer:
		public Utils::Singleton<BNetTCPServer>,
		public Network::SocketServer
	{
	protected:
		virtual Network::TcpClient * OnNewTcpClient(int sock);
	};
}

#endif // _BNetTCPSERVER_H_

