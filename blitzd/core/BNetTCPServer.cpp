#include "Config.h"
#include "BNetTCPServer.h"
#include "Client.h"

namespace Core
{
	Network::TcpClient * BNetTCPServer::OnNewTcpClient( int sock )
	{
		/*
		sockaddr_in addr;
		socklen_t len = sizeof(sockaddr_in);
		getpeername(sock, (sockaddr *)&addr, &len);
		LOG_DEBUG(("connected from %08X", addr.sin_addr.s_addr));
		*/
		return new Client(sock);
	}
}
