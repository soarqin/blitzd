#include "Config.h"

#include "SocketServer.iocp.h"
#include "TcpClient.iocp.h"
#include "utils/Memory.h"
#include "utils/Runnable.h"

#define ACCEPTER_MAX 16

namespace Network
{
	class _WorkerRunner:
		public Utils::Runnable
	{
	public:
		virtual void run()
		{
			ts->Worker();
		}
		SocketServer * ts;
	};

	class _AccepterRunner:
		public Utils::Runnable
	{
	public:
		virtual void run()
		{
			ts->Accepter();
		}
		SocketServer * ts;
	};

	SocketServer::SocketServer(): _hIOCP(NULL), _hShutdown(NULL)
	{
	}

	SocketServer::~SocketServer()
	{
		Destroy();
	}

	bool SocketServer::Init( ushort port )
	{
		InitIOCP();

		if(!AddListener(port))
		{
			Destroy();
			return false;
		}

		return true;
	}

	bool SocketServer::Init( std::vector<ushort> ports )
	{
		if(ports.empty())
			return false;

		InitIOCP();

		size_t size = ports.size();
		for(size_t i = 0; i < size; ++ i)
		{
			AddListener(ports[i]);
		}

		return true;
	}

	void SocketServer::Destroy()
	{
		if(!_fd_listen.empty())
		{
			size_t size = _fd_listen.size();
			for(size_t i = 0; i < size; ++ i)
			{
				shutdown(_fd_listen[i], SHUT_WR);
				close(_fd_listen[i]);
			}
			_fd_listen.clear();
		}
		SetEvent(_hShutdown);
		if(_worker.isRunning())
		{
			_worker.join();
		}
		if(_accepter.isRunning())
		{
			_accepter.join();
		}
		WSACloseEvent(_hShutdown);
	}

	void SocketServer::Worker()
	{
		while(WaitForSingleObject(_hShutdown, 0) != WAIT_OBJECT_0)
		{
			DWORD dwBytes;
			ULONG_PTR key;
			LPOVERLAPPED overlapped;

			BOOL r = GetQueuedCompletionStatus(_hIOCP, &dwBytes, &key, &overlapped, INFINITE);
			if(key == NULL)
				break;
			IO_DATA * iodata = (IO_DATA *)overlapped;
			if(!r || dwBytes == 0)
			{
				SocketBase * sbase;
				if(iodata->op == OP_SEND)
					sbase = (SocketBase *)(((byte *)overlapped) - offsetof(SocketBase, io_send));
				else
					sbase = (SocketBase *)(((byte *)overlapped) - offsetof(SocketBase, io_recv));
				SOCKET s = sbase->_fd;
				Close(s);
				continue;
			}
			switch(iodata->op)
			{
			case OP_ACCEPT:
				{
					SocketBase * sbase = (SocketBase *)(((byte *)overlapped) - offsetof(SocketBase, io_recv));
					setsockopt(sbase->_fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (const char *)&key, sizeof(SOCKET));
					sbase->_Register(this);
					sbase->_DoRecv(dwBytes);
				}
				break;
			case OP_RECV:
				{
					SocketBase * sbase = (SocketBase *)(((byte *)overlapped) - offsetof(SocketBase, io_recv));
					sbase->_DoRecv(dwBytes);
				}
				break;
			case OP_SEND:
				{
					SocketBase * sbase = (SocketBase *)(((byte *)overlapped) - offsetof(SocketBase, io_send));
					sbase->_DoSend(dwBytes);
				}
				break;
			}
		}
	}

	TcpClient * SocketServer::OnNewTcpClient( int fd )
	{
		return new TcpClient(fd);
	}

	void SocketServer::Close( int fd )
	{
		_fd_map.erase(fd);
	}

	void SocketServer::RegisterTimer( uint tm, SocketServer::ITimer * timer )
	{
		if(timer->_timer != NULL)
			DeleteTimerQueueTimer(NULL, timer->_timer, NULL);
		timer->_timer = NULL;
		CreateTimerQueueTimer(&timer->_timer, NULL, ITimer::TimerProc, timer, tm, tm, 0);
	}

	void SocketServer::Run()
	{
		static _AccepterRunner accepterrunner;
		static _WorkerRunner workerrunner;
		accepterrunner.ts = this;
		workerrunner.ts = this;
		_accepter.start(accepterrunner);
		_worker.start(workerrunner);
	}

	void SocketServer::AddSocket( SocketBase * cl )
	{
		_fd_map[*cl] = SocketBase::Pointer(cl);
	}

	bool SocketServer::AddListener( ushort port )
	{
		SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if(listenSocket == INVALID_SOCKET)
			return false;

		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		if(bind(listenSocket, (const SOCKADDR *)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR
			|| listen(listenSocket, ACCEPTER_MAX) == SOCKET_ERROR
			|| CreateIoCompletionPort((HANDLE)listenSocket, _hIOCP, (ULONG_PTR)listenSocket, 0) == NULL)
		{
			closesocket(listenSocket);
			return false;
		}

		_fd_listen.push_back(listenSocket);

		return true;
	}

	void SocketServer::Accepter()
	{
		std::vector<WSAEVENT> hAcceptEvent;
		size_t count = _fd_listen.size();
		for(size_t i = 0; i < count; ++ i)
		{
			WSAEVENT ev = WSACreateEvent();
			WSAEventSelect(_fd_listen[i], ev, FD_ACCEPT);
			hAcceptEvent.push_back(ev);
			PushAccept(_fd_listen[i]);
		}
		while(WaitForSingleObject(_hShutdown, 0) != WAIT_OBJECT_0)
		{
			DWORD result = WSAWaitForMultipleEvents(count, &hAcceptEvent[0], FALSE, INFINITE, FALSE);
			if(result >= WSA_WAIT_EVENT_0 && result < WSA_WAIT_EVENT_0 + count)
			{
				size_t idx = result - WSA_WAIT_EVENT_0;
				PushAccept(_fd_listen[idx]);
			}
		}
		for(size_t i = 0; i < count; ++ i)
			WSACloseEvent(hAcceptEvent[i]);
		hAcceptEvent.clear();
	}

	bool SocketServer::InitIOCP()
	{
		_hShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
		_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		return _hIOCP != NULL;
	}

	void SocketServer::PushAccept( int listenfd )
	{
		for(int i = 0; i < ACCEPTER_MAX; ++ i)
		{
			int fd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
			if(fd == INVALID_SOCKET)
				continue;
			TcpClient * tcpclient = (TcpClient *)OnNewTcpClient(fd);
			tcpclient->_WaitAccept(listenfd);
		}
	}
}
