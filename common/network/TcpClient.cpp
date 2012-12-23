#include "Config.h"

#if defined(WIN32) && defined(USE_IOCP)

#include "TcpClient.iocp.cpp"

#else

#include "TcpClient.h"
#include "SocketServer.h"
#include "utils/Memory.h"
#ifdef WIN32
#include "../libevent/event.h"
#else
#include "event.h"
#endif

namespace Network
{
	TcpClient::TcpClient( int fd ): SocketBase(fd), _pend_close(false), _send_pos(0)
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

#define MTU_MIN_SIZE 1440
#define PACKET_SIZE_MAX 65535
#define ALIGN_LEN(l) ((((l) + 4) + 0x3F) & ~0x3F)
	void TcpClient::Send( const byte * data, size_t len )
	{
		if(len == 0 || len > PACKET_SIZE_MAX)
			return;
		if(_send_queue.empty())
		{
			struct event * ev = (struct event *)_w_event;
			event_add(ev, NULL);
		}

		if(_send_queue.empty() || _send_queue.back()->len + len > MTU_MIN_SIZE)
		{
			do
			{
				if(len > MTU_MIN_SIZE)
				{
					_packet_t * p = (_packet_t *)_MEM.Alloc(4 + MTU_MIN_SIZE);
					memcpy(p->buf, data, MTU_MIN_SIZE);
					p->len = MTU_MIN_SIZE;
					_send_queue.push(p);
					len -= MTU_MIN_SIZE;
					data += MTU_MIN_SIZE;
				}
				else
				{
					_packet_t * p = (_packet_t *)_MEM.Alloc(ALIGN_LEN(len));
					memcpy(p->buf, data, len);
					p->len = len;
					_send_queue.push(p);
					len = 0;
				}
			}
			while(len > 0);
		}
		else
		{
			_packet_t *& p = _send_queue.back();
			size_t oldlen_pad = ALIGN_LEN(p->len);
			size_t newlen = p->len + len;
			size_t newlen_pad = ALIGN_LEN(newlen);
			if(newlen_pad > oldlen_pad)
			{
				p = (_packet_t *)_MEM.ReAlloc(p, newlen_pad);
			}
			memcpy(p->buf + p->len, data, len);
			p->len = newlen;
		}
	}

	void TcpClient::_DoRecv()
	{
		char buf[2048];
		int len = recv(_fd, buf, 2048, 0);
		if(len == 0)
		{
			if(_host != NULL)
				_host->Close(_fd);
			return;
		}
		if(len < 0)
		{
			int lerror = SOCKET_ERRNO();
			switch(lerror)
			{
			case ENOMEM:
#ifdef _WIN32
			case EAGAIN:
#endif
			case EINTR:
			case EWOULDBLOCK:
			case EFAULT:
				break;
			default:
				Close();
				return;
			}
		}

		OnRecv((byte *)buf, (size_t)len);
		event_add((struct event *)_r_event, NULL);
	}

	void TcpClient::_DoSend()
	{
		while(!_send_queue.empty())
		{
			_packet_t * p = _send_queue.front();
			int len = send(_fd, (const char *)p->buf + _send_pos, (int)(p->len - _send_pos), 0);
			if(len < 0)
			{
				int lerror = SOCKET_ERRNO();
				switch(lerror)
				{
				case ENOMEM:
#ifdef _WIN32
				case EAGAIN:
#endif
				case EINTR:
				case EWOULDBLOCK:
				case EFAULT:
					break;
				default:
					Close();
					return;
				}
				break;
			}
			else if(len > 0)
			{
				_send_pos += (size_t)len;
				if(p->len <= _send_pos)
				{
					_MEM.Free(p);
					_send_queue.pop();
					_send_pos = 0;
					continue;
				}
			}
			break;
		}
		if(_send_queue.empty())
		{
			if(_pend_close)
			{
				_pend_close = false;
				Close();
			}
		}
		else
		{
			event_add((struct event *)_w_event, NULL);
		}
	}

	void TcpClient::PendClose()
	{
		if(_send_queue.empty())
		{
			Close();
			return;
		}
		_pend_close = true;
	}

	void TcpClient::ClearSendQueue()
	{
		if(!_send_queue.empty())
		{
			event_del((struct event *)_w_event);
			do
			{
				_packet_t * p = _send_queue.front();
				_MEM.Free(p);
				_send_queue.pop();
			}
			while(!_send_queue.empty());
		}
		_send_pos = 0;
	}
}

#endif
