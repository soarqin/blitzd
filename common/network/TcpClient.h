#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#include "SocketBase.h"

namespace Network
{
	class TcpClient:
		public SocketBase
	{
	public:
		TcpClient(int fd);
		TcpClient(uint ip, ushort port);
		virtual ~TcpClient();
		void Send(const byte * data, size_t len);
		void PendClose();
	protected:
		virtual void OnRecv(SocketBuf&) {}

		virtual void DoRecv();
		virtual void DoSend();
		virtual void DoDisconnect();
		virtual bool DoRegister();

	private:
		static void _client_write_cb(struct bufferevent *, void *);
		static void _client_read_cb(struct bufferevent *, void *);
		static void _client_event_cb(struct bufferevent *, short, void *);

	private:
		bool _pend_close;
		struct bufferevent * _bufev;
	};
}

#endif // _TCPCLIENT_H_
