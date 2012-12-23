#ifndef _GAMESERVER_H_
#define _GAMESERVER_H_

#include "network/SocketBase.h"
#include "network/SocketServer.h"
#include "utils/Singleton.h"

namespace HostBot
{
	class GameServer:
		public Utils::Singleton<GameServer>,
		public Network::SocketServer
	{
	public:
	protected:
		virtual Network::TcpClient * OnNewTcpClient(int sock);
	};
}

#endif // _GAMESERVER_H_
