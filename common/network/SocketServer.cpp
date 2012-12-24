#include "Config.h"

#if defined(WIN32) && defined(USE_IOCP)

#include "SocketServer.iocp.cpp"

#else

#include "SocketServer.h"
#include "TcpClient.h"
#include "utils/Memory.h"
#include "utils/Runnable.h"
#include <event2/event.h>
#ifdef WIN32

#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

void CreateMiniDump( LPEXCEPTION_POINTERS lpExceptionInfo) {
    // Open a file
    HANDLE hFile = CreateFileA("MiniDump.dmp", GENERIC_READ | GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if ( hFile != NULL &&  hFile != INVALID_HANDLE_VALUE ) {

        // Create the minidump 
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId          = GetCurrentThreadId();
        mdei.ExceptionPointers = lpExceptionInfo;
        mdei.ClientPointers    = FALSE;

        MINIDUMP_TYPE mdt      = MiniDumpNormal;
        BOOL retv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
            hFile, mdt, ( lpExceptionInfo != 0 ) ? &mdei : 0, 0, 0);
        
        if ( !retv ) {
            printf( "MiniDumpWriteDump failed. Error: %u \n", GetLastError() ); 
        } else { 
            printf( "Minidump created.\n" ); 
        }

        // Close the file
        CloseHandle( hFile );

    } else {
        printf( "CreateFile failed. Error: %u \n", GetLastError() ); 
    }
}

#endif

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

	void SocketServer::_listen_cb( int fd, short events, void * arg )
	{
		if(events & EV_READ)
		{
			SocketServer * server = (SocketServer *)arg;
			server->Accept(fd);
		}
		else if(events & EV_WRITE)
		{
			SocketServer * server = (SocketServer *)arg;
			event_base_loopbreak(server->_base);
		}
	}

	void SocketServer::_timer_cb( int fd, short events, void * arg )
	{
		SocketServer::ITimer * timer = (SocketServer::ITimer *)arg;
		if(timer != NULL)
		{
			if(!(*timer)())
			{
				_MEM.Free(timer->_event);
				timer->_event = NULL;
			}
			else
			{
				evtimer_add(timer->_event, &timer->_tv);
			}
		}
	}

	SocketServer::SocketServer(): _base(NULL), _fd_listen(0)
	{
	}

	SocketServer::~SocketServer()
	{
		Destroy();
	}

	bool SocketServer::Init( ushort port )
	{
		_base = event_base_new();
		if(_base == NULL)
			return false;

		_fd_map.clear();
		_fd_listen.clear();
		_fd_events.clear();

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

		_base = event_base_new();
		if(_base == NULL)
			return false;

		_fd_map.clear();
		_fd_listen.clear();
		_fd_events.clear();

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
		if(_worker.isRunning())
		{
			event_base_loopbreak(_base);
			_worker.join();
		}
		if(!_fd_events.empty())
		{
			size_t size = _fd_events.size();
			for(size_t i = 0; i < size; ++ i)
			{
				_MEM.Free(_fd_events[i]);
			}
			_fd_events.clear();
		}
		if(_base != NULL)
		{
			event_base_free(_base);
			_base = NULL;
		}
	}

	void SocketServer::Accept(int fd)
	{
		sockaddr_in addr;
		socklen_t size = sizeof(sockaddr_in);
		int new_fd = accept(fd, (sockaddr *)&addr, &size);
		if(new_fd == -1)
			return;
		evutil_make_socket_nonblocking(fd);
		TcpClient * cl = OnNewTcpClient(new_fd);
		if(cl != NULL)
		{
			cl->_Register(this);
		}
	}

	void SocketServer::Worker()
	{
#ifdef _WIN32
		__try {
#endif
			event_base_loop(_base, 0);
#ifdef _WIN32
		}
		__except(CreateMiniDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {
			exit(0);
		}
#endif
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
		timer->_event = (struct event *)_MEM.Alloc(sizeof(struct event));
		evtimer_set(timer->_event, _timer_cb, timer);
		event_base_set(_base, timer->_event);
		timer->_tv.tv_sec = tm / 1000;
		timer->_tv.tv_usec = (tm % 1000) * 1000;
		evtimer_add(timer->_event, &timer->_tv);
	}

	void SocketServer::Run()
	{
		static _WorkerRunner runner;
		runner.ts = this;
		_worker.start(runner);
	}

	void SocketServer::AddSocket( SocketBase * cl )
	{
		_fd_map[*cl] = SocketBase::Pointer(cl);
	}

	bool SocketServer::AddListener( ushort port )
	{
		struct event * _listen_event = (struct event *)_MEM.Alloc(sizeof(struct event));
		if(_listen_event == NULL)
			return false;

		int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(fd == -1)
		{
			_MEM.Free(_listen_event);
			return false;
		}

		evutil_make_socket_nonblocking(fd);

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons( port );
		addr.sin_addr.s_addr = INADDR_ANY;
		if(SOCKET_ERROR == ::bind( fd, (sockaddr *)&addr, sizeof( addr )) || SOCKET_ERROR == listen( fd, 20 ))
		{
			_MEM.Free(_listen_event);
			return false;
		}

		event_set(_listen_event, fd, EV_READ | EV_WRITE | EV_PERSIST, _listen_cb, this);
		event_base_set(_base, _listen_event);
		event_add(_listen_event, NULL);

		_fd_listen.push_back(fd);
		_fd_events.push_back(_listen_event);

		return true;
	}
}

#endif
