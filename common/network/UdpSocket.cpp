#include "Config.h"

#if defined(WIN32) && defined(USE_IOCP)

#include "UdpSocket.iocp.cpp"

#else

#include "UdpSocket.h"
#include "utils/Runnable.h"
#ifdef WIN32
#include "../libevent/event.h"
#else
#include "event.h"
#endif

namespace Network
{
	UdpSocket::UdpSocket()
	{
	}

	UdpSocket::~UdpSocket()
	{
	}

	bool UdpSocket::Init( ushort port, SocketServer * host )
	{
		_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(_fd == -1)
		{
			_fd = 0;
			return false;
		}
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
		_send_data_t data;
		data.toip = ip;
		data.toport = port;
		data.buffer = buffer;
		if(_packet_queue.empty())
		{
			struct event * ev = (struct event *)_w_event;
			event_add(ev, NULL);
		}
		_packet_queue.push(data);
	}

	void UdpSocket::_DoRecv()
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
		event_add((struct event *)_r_event, NULL);
	}

	void UdpSocket::_DoSend()
	{
		while(!_packet_queue.empty())
		{
			_send_data_t& data = _packet_queue.front();
			if(!data.buffer.empty())
			{
				sockaddr_in addr = {0};
				addr.sin_family = AF_INET;
				addr.sin_port = htons(data.toport);
				addr.sin_addr.s_addr = htonl(data.toip);
				if(sendto(_fd, (const char *)&data.buffer[0], (int)data.buffer.size(), 0, (const sockaddr *)&addr, sizeof(sockaddr_in)) <= 0)
					break;
			}
			_packet_queue.pop();
		}
	}
}

#endif
