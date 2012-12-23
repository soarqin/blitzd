#include "Config.h"
#include "UdpSocket.iocp.h"
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
		_fd = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
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
			return false;
		}

		_Register(host, false);

		io_recv.op = OP_RECV;
		WSABUF wsabuf;
		wsabuf.buf = (char *)input_buf;
		wsabuf.len = 1536 - sizeof(SOCKADDR_IN) - sizeof(int);
		DWORD dwFlags = 0;
		int * addrLen = (int *)(input_buf + (1536 - sizeof(int)));
		* addrLen = sizeof(SOCKADDR_IN);
		WSARecvFrom(_fd, &wsabuf, 1, NULL, &dwFlags, (SOCKADDR *)(input_buf + (1536 - sizeof(SOCKADDR_IN) - sizeof(int))), addrLen, &io_recv.overlapped, NULL);

		return true;
	}

	void UdpSocket::Broadcast( ushort port, std::vector<byte>& buffer )
	{
		Send(INADDR_BROADCAST, port, buffer);
	}

	void UdpSocket::Send( uint ip, ushort port, std::vector<byte>& buffer )
	{
		Utils::FastMutex::ScopedLock lock(_pq_mutex);
		_send_data_t data;
		data.toaddr.sin_family = AF_INET;
		data.toaddr.sin_addr.s_addr = htonl(ip);
		data.toaddr.sin_port = htons(port);
		data.buffer = buffer;
		_packet_queue.push_back(data);
		if(io_send.op == OP_NONE)
		{
			io_send.op = OP_SEND;
			_send_data_t& sd = _packet_queue.front();
			WSABUF wsabuf;
			wsabuf.buf = (char *)&sd.buffer[0];
			wsabuf.len = _packet_queue.front().buffer.size();
			WSASendTo(_fd, &wsabuf, 1, NULL, 0, (const SOCKADDR *)&sd.toaddr, sizeof(SOCKADDR_IN), &io_send.overlapped, NULL);
		}
	}

	void UdpSocket::_DoRecv(DWORD size)
	{
		SOCKADDR_IN * addr = (SOCKADDR_IN *)(input_buf + (1536 - sizeof(SOCKADDR_IN) - sizeof(int)));
		OnRecv(ntohl(addr->sin_addr.s_addr), ntohs(addr->sin_port), input_buf, size);

		io_recv.op = OP_RECV;
		WSABUF wsabuf;
		wsabuf.buf = (char *)input_buf;
		wsabuf.len = 1536 - sizeof(SOCKADDR_IN) - sizeof(int);
		DWORD dwFlags = 0;
		int * addrLen = (int *)(input_buf + (1536 - sizeof(int)));
		* addrLen = sizeof(SOCKADDR_IN);
		WSARecvFrom(_fd, &wsabuf, 1, NULL, &dwFlags, (SOCKADDR *)(input_buf + (1536 - sizeof(SOCKADDR_IN) - sizeof(int))), addrLen, &io_recv.overlapped, NULL);
	}

	void UdpSocket::_DoSend(DWORD)
	{
		Utils::FastMutex::ScopedLock lock(_pq_mutex);
		if(!_packet_queue.empty())
			_packet_queue.pop_front();
		if(_packet_queue.empty())
		{
			io_send.op = OP_NONE;
		}
		else
		{
			io_send.op = OP_SEND;
			WSABUF wsabuf;
			_send_data_t& sd = _packet_queue.front();
			wsabuf.buf = (char *)&sd.buffer[0];
			wsabuf.len = sd.buffer.size();
			WSASendTo(_fd, &wsabuf, 1, NULL, 0, (const SOCKADDR *)&sd.toaddr, sizeof(SOCKADDR_IN), &io_send.overlapped, NULL);
		}
	}
}
