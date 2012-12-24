#include "Config.h"
#include "TcpClient.iocp.h"
#include "SocketServer.iocp.h"
#include "utils/Memory.h"

namespace Network
{
	TcpClient::TcpClient( int fd ): SocketBase(fd), output_offset(0), _pend_close(false)
	{
	}

	TcpClient::TcpClient(uint ip, ushort port): SocketBase(0), _pend_close(false)
	{
		int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(fd == -1)
			throw std::bad_exception();
		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(ip);
		addr.sin_port = htons(port);
		if(connect(fd, (const sockaddr *)&addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			throw std::bad_exception();
		}
		_fd = fd;
	}

	TcpClient::~TcpClient()
	{
		ClearSendQueue();
	}

#define PACKET_SIZE_MAX 65535
	void TcpClient::Send( const byte * data, size_t len )
	{
		if(len == 0 || len > PACKET_SIZE_MAX)
			return;

		output_buf.insert(output_buf.end(), data, data + len);
		bool needsend = io_send.op == OP_NONE;
		if(needsend)
		{
			io_send.op = OP_SEND;
			WSABUF wsabuf;
			wsabuf.buf = (char *)&output_buf[output_offset];
			wsabuf.len = output_buf.size() - output_offset;
			int n = WSASend(_fd, &wsabuf, 1, NULL, 0, &(io_send.overlapped), NULL);
			if(n == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				shutdown(_fd, SD_BOTH);
			}
		}
	}

	void TcpClient::_DoRecv(DWORD size)
	{
		if(size > 0)
			OnRecv(input_buf, size);
		io_recv.op = OP_RECV;
		WSABUF wsabuf;
		wsabuf.buf = (char *)input_buf;
		wsabuf.len = BUF_MAX;
		DWORD dwFlags = 0;
		int n = WSARecv(_fd, &wsabuf, 1, NULL, &dwFlags, &(io_recv.overlapped), NULL);
		if(n == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
		{
			shutdown(_fd, SD_BOTH);
		}
	}

	void TcpClient::_DoSend(DWORD size)
	{
		output_offset += size;
		if(output_offset >= output_buf.size())
		{
			ClearSendQueue();
			if(_pend_close)
			{
				shutdown(_fd, SD_BOTH);
			}
		}
		else
		{
			io_send.op = OP_SEND;
			WSABUF wsabuf;
			wsabuf.buf = (char *)&output_buf[output_offset];
			wsabuf.len = output_buf.size() - output_offset;
			int n = WSASend(_fd, &wsabuf, 1, NULL, 0, &(io_send.overlapped), NULL);
			if(n == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				shutdown(_fd, SD_BOTH);
			}
		}
	}

	void TcpClient::PendClose()
	{
		if(io_send.op == OP_NONE)
		{
			shutdown(_fd, SD_BOTH);
			return;
		}
		_pend_close = true;
	}

	void TcpClient::ClearSendQueue()
	{
		io_send.op = OP_NONE;
		output_buf.clear();
		output_offset = 0;
	}

	void TcpClient::_WaitAccept(int listenfd)
	{
		ZeroMemory(&io_recv.overlapped, sizeof(OVERLAPPED));
		io_recv.op = OP_ACCEPT;
		DWORD dwBytes;
		BOOL r = AcceptEx(listenfd, _fd, input_buf, BUF_MAX - (sizeof(SOCKADDR_IN) + 16) * 2, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, &io_recv.overlapped);
		if(!r && WSAGetLastError() != ERROR_IO_PENDING)
		{
			_host->Close(_fd);
		}
	}
}
