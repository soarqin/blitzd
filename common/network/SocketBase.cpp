#include "Config.h"

#include "SocketBase.h"
#include "SocketServer.h"

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

namespace Network
{
	SocketBase::SocketBase(int fd): _fd(fd), _host(0)
	{

	}

	SocketBase::~SocketBase()
	{
		if(_fd != 0)
		{
			close(_fd);
			_fd = 0;
		}
	}

	bool SocketBase::_Register( SocketServer * svr, bool add )
	{
		_host = svr;
		if(!DoRegister())
			return false;
		if(add)
			svr->AddSocket(this);
		return true;
	}

	void SocketBase::Close()
	{
		_host->Close(_fd);
	}


	size_t SocketBuf::length()
	{
		return evbuffer_get_length(_bev);
	}

	int SocketBuf::copyout( void * data, size_t datlen )
	{
		return evbuffer_copyout(_bev, data, datlen);
	}

	int SocketBuf::read( void * data, size_t datlen )
	{
		return evbuffer_remove(_bev, data, datlen);
	}

	void SocketBuf::drain( size_t datlen )
	{
		evbuffer_drain(_bev, datlen);
	}

}
