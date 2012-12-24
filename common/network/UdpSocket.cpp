#include "Config.h"

#include "UdpSocket.h"
#include "SocketServer.h"
#include "utils/Runnable.h"
#include <event2/event.h>

namespace Network
{
	UdpSocket::UdpSocket(): _ev(NULL)
	{
	}

	UdpSocket::~UdpSocket()
	{
		if(_ev != NULL)
		{
			event_del(_ev);
			event_free(_ev);
		}
	}

	bool UdpSocket::Init( ushort port, SocketServer * host )
	{
		_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(_fd == -1)
		{
			_fd = 0;
			return false;
		}
		evutil_make_listen_socket_reuseable(_fd);
		int bOptVal = 1;
		setsockopt(_fd, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, sizeof(int));
		sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		if(bind(_fd, (const sockaddr *)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			Close();
			close(_fd);
			return false;
		}
		_Register(host, false);
		return true;
	}

	void UdpSocket::Broadcast( ushort port, std::vector<byte>& buffer )
	{
		Send(INADDR_BROADCAST, port, buffer);
	}

	void UdpSocket::Send( uint ip, ushort port, std::vector<byte>& buffer )
	{
		if(buffer.empty())
			return;
		sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(ip);
		sendto(_fd, (const char *)&buffer[0], (int)buffer.size(), 0, (const sockaddr *)&addr, sizeof(sockaddr_in));
	}

	void UdpSocket::DoRecv()
	{
		byte buf[2048];
		sockaddr_in addr = {0};
		socklen_t addrlen = sizeof(sockaddr_in);
		int len = recvfrom(_fd, (char *)buf, 2048, 0, (sockaddr *)&addr, &addrlen);
		if(len < 0)
		{
			int err = SOCKET_ERRNO();
			if (err != EINTR && err != EWOULDBLOCK && err != EAGAIN && err != ECONNRESET && err != ENETRESET)
			{
				Close();
			}
		}
		if(len > 0)
		{
			OnRecv(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port), buf, len);
		}
	}

	bool UdpSocket::DoRegister()
	{
		struct event * ev = event_new(_host->_base, _fd, EV_READ | EV_PERSIST, _client_read_cb, this);
		if(ev == NULL)
			return false;
		event_add(ev, NULL);
		return true;
	}

	void UdpSocket::_client_read_cb( int fd, short op, void * arg )
	{
		UdpSocket * s = (UdpSocket *)arg;
		s->DoRecv();
	}

}
