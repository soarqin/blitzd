#include "Config.h"

#include "TcpClient.h"
#include "SocketServer.h"
#include "utils/Memory.h"
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

namespace Network
{
	TcpClient::TcpClient( int fd ): SocketBase(fd), _pend_close(false), _bufev(0)
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
		if(_bufev != NULL)
		{
			bufferevent_free(_bufev);
		}
	}

#define MTU_MIN_SIZE 1440
#define PACKET_SIZE_MAX 65535
#define ALIGN_LEN(l) ((((l) + 4) + 0x3F) & ~0x3F)
	void TcpClient::Send( const byte * data, size_t len )
	{
		bufferevent_write(_bufev, data, len);
	}

	void TcpClient::PendClose()
	{
		if(evbuffer_get_length(bufferevent_get_output(_bufev)) == 0)
		{
			Close();
			return;
		}
		_pend_close = true;
	}

	void TcpClient::DoRecv()
	{
		SocketBuf sbuf(bufferevent_get_input(_bufev));
		OnRecv(sbuf);
	}

	void TcpClient::DoSend()
	{
		if(_pend_close && evbuffer_get_length(bufferevent_get_output(_bufev)) == 0)
		{
			_pend_close = false;
			Close();
		}
	}

	void TcpClient::DoDisconnect()
	{
		Close();
	}

	bool TcpClient::DoRegister( )
	{
		evutil_make_socket_nonblocking(_fd);
		bufferevent * bev = bufferevent_socket_new(_host->_base, _fd, BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE | BEV_OPT_CLOSE_ON_FREE);
		if(bev == NULL)
			return false;
		bufferevent_setcb(bev, _client_read_cb, _client_write_cb, _client_event_cb, this);
		bufferevent_enable(bev, EV_READ | EV_WRITE);
		_bufev = bev;
		return true;
	}

	void TcpClient::_client_read_cb( struct bufferevent * bev, void * arg )
	{
		TcpClient * client = (TcpClient *)arg;
		client->DoRecv();
	}

	void TcpClient::_client_write_cb( struct bufferevent * bev, void * arg )
	{
		TcpClient * client = (TcpClient *)arg;
		client->DoSend();
	}

	void TcpClient::_client_event_cb( struct bufferevent * bev, short wht, void * arg )
	{
		if((wht & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) > 0)
		{
			TcpClient * client = (TcpClient *)arg;
			bufferevent_disable(bev, EV_WRITE);
			client->DoDisconnect();
		}
	}

}
