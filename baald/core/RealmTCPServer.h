#ifndef _REALMTCPSERVER_H_
#define _REALMTCPSERVER_H_

#include "utils/Singleton.h"
#include "network/SocketServer.h"

namespace Core
{
	class RealmTCPServer:
		public Utils::Singleton<RealmTCPServer>,
		public Network::SocketServer
	{
	protected:
		virtual Network::TcpClient * OnNewTcpClient(int sock);
	};
}

#endif // _REALMTCPSERVER_H_
