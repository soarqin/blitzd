#ifndef _SOCKETBASE_IOCP_H_
#define _SOCKETBASE_IOCP_H_

#include "utils/SharedPtr.h"

namespace Network
{
	enum IO_DATA_OP {
		OP_NONE = 0,
		OP_ACCEPT = 1,
		OP_RECV = 2,
		OP_SEND = 3
	};
	struct IO_DATA
	{
		OVERLAPPED overlapped;
		IO_DATA_OP op;

		IO_DATA(): op(OP_NONE) {ZeroMemory(&overlapped, sizeof(OVERLAPPED));}
	};
    class SocketServer;
	class SocketBase;

	class SocketBase:
		public Utils::SharedClass<SocketBase>
	{
		friend class SocketServer;
	protected:
		IO_DATA io_recv;
		IO_DATA io_send;

	public:
		SocketBase(int fd = 0);
		virtual ~SocketBase();

		void Close();
		operator int() {return _fd;}
		virtual void OnDisconnect() {}
	protected:
		int _fd;
		SocketServer * _host;

		bool _Register(SocketServer *, bool add = true);
		virtual void _DoRecv(DWORD) = 0;
		virtual void _DoSend(DWORD) = 0;
	};
}

#endif // _SOCKETBASE_IOCP_H_
