#ifndef _SHALTCPSERVER_H_
#define _SHALTCPSERVER_H_

#include "utils/Singleton.h"
#include "network/SocketServer.h"

namespace Core
{
	class ShalTCPServer:
		public Utils::Singleton<ShalTCPServer>,
		public Network::SocketServer
	{
	protected:
		virtual Network::TcpClient * OnNewTcpClient(int sock);
	};
}

#endif // _SHALTCPSERVER_H_
