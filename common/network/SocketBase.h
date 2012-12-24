#ifndef _SOCKETBASE_H_
#define _SOCKETBASE_H_

#include "utils/SharedPtr.h"

struct bufferevent;
struct evbuffer;

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

	protected:
		bool _Register(SocketServer *, bool add = true);
		virtual bool DoRegister() = 0;

	protected:
		int _fd;
		SocketServer * _host;
	};

	class SocketBuf
	{
	public:
		SocketBuf(struct evbuffer * bev): _bev(bev) { }
		size_t length();
		int copyout(void * data, size_t datlen);
		int read(void * data, size_t datlen);
		void drain(size_t datlen);
	private:
		struct evbuffer * _bev;
	};
}

#endif // _SOCKETBASE_H_
