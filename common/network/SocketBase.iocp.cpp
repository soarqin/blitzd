#include "Config.h"
#include "SocketBase.iocp.h"
#include "SocketServer.iocp.h"

namespace Network
{
	SocketBase::SocketBase(int fd): _fd(fd), _host(0)
	{
		ZeroMemory(&io_recv.overlapped, sizeof(OVERLAPPED));
		ZeroMemory(&io_send.overlapped, sizeof(OVERLAPPED));
	}

	SocketBase::~SocketBase()
	{
		if(_fd != 0)
		{
//			shutdown(_fd, SHUT_WR);
			OnDisconnect();
			close(_fd);
			_fd = 0;
		}
	}

	bool SocketBase::_Register( SocketServer * svr, bool add )
	{
		if(CreateIoCompletionPort((HANDLE)_fd, svr->_hIOCP, (ULONG_PTR)_fd, 0) == NULL)
			return false;
		if(add)
			svr->AddSocket(this);
		return true;
	}

	void SocketBase::Close()
	{
		_host->Close(_fd);
	}
}
