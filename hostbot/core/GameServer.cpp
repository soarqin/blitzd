#include "Config.h"
#include "GameServer.h"
#include "Player.h"

namespace HostBot
{
	Network::TcpClient * GameServer::OnNewTcpClient( int fd )
	{
		return new Player(fd);
	}
}
