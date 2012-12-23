#include "Config.h"
#include "RealmTCPServer.h"
#include "Client.h"

namespace Core
{
	Network::TcpClient * RealmTCPServer::OnNewTcpClient( int sock )
	{
		return new Client(sock);
	}
}
