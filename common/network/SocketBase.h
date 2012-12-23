#ifndef _SOCKETBASE_H_
#define _SOCKETBASE_H_

#if defined(WIN32) && defined(USE_IOCP)

#include "SocketBase.iocp.h"

#else

#include "utils/SharedPtr.h"

struct event_base;

namespace Network
{
    class SocketServer;
	class SocketBase:
		public Utils::SharedClass<SocketBase>
	{
		friend class SocketServer;
	public:
		SocketBase(int fd = 0);
		virtual ~SocketBase();

		void Close();
		operator int() {return _fd;}
		virtual void OnDisconnect() {}
	protected:
		int _fd;
		struct event_base * _base;
		void * _r_event, * _w_event;
		SocketServer * _host;

		void _Register(SocketServer *, bool add = true);
		virtual void _DoRecv() = 0;
		virtual void _DoSend() = 0;

		static void _client_write_cb(int, short, void *);
		static void _client_read_cb(int, short, void *);
	};
}

#endif

#endif // _SOCKETBASE_H_
