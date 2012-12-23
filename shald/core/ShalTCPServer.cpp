#include "Config.h"
#include "ShalTCPServer.h"
#include "Client.h"

namespace Core
{
	Network::TcpClient * ShalTCPServer::OnNewTcpClient( int sock )
	{
		return new Client(sock);
	}
}
