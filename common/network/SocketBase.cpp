#include "Config.h"

#if defined(WIN32) && defined(USE_IOCP)

#include "SocketBase.iocp.cpp"

#else

#include "SocketBase.h"
#include "SocketServer.h"

#include <event2/event.h>

namespace Network
{
	SocketBase::SocketBase(int fd): _fd(fd), _base(0), _r_event(0), _w_event(0), _host(0)
	{

	}

	SocketBase::~SocketBase()
	{
		if(_r_event != NULL)
		{
			event_del((struct event *)_r_event);
			_MEM.Free((struct event *)_r_event);
			_r_event = NULL;
		}
		if(_w_event != NULL)
		{
			event_del((struct event *)_w_event);
			_MEM.Free((struct event *)_w_event);
			_w_event = NULL;
		}
		if(_fd != 0)
		{
//			shutdown(_fd, SHUT_WR);
			OnDisconnect();
			close(_fd);
			_fd = 0;
		}
	}

	void SocketBase::_Register( SocketServer * svr, bool add )
	{
		_base = svr->_base;
		_host = svr;

		struct event * ev = event_new(_base, _fd, EV_READ, _client_read_cb, this);
		if(ev == NULL)
			return;
		struct event * ev2 = event_new(_base, _fd, EV_WRITE, _client_write_cb, this);
		if(ev2 == NULL)
		{
			event_free(ev2);
			return;
		}
		_r_event = (void *)ev;
		_w_event = (void *)ev2;

		event_add(ev, NULL);

		if(add)
			svr->AddSocket(this);
	}

	void SocketBase::Close()
	{
		_host->Close(_fd);
	}

	void SocketBase::_client_read_cb( int fd, short events, void * arg )
	{
		SocketBase * client = (SocketBase *)arg;
		client->_DoRecv();
	}
	void SocketBase::_client_write_cb( int fd, short events, void * arg )
	{
		SocketBase * client = (SocketBase *)arg;
		client->_DoSend();
	}
}

#endif
