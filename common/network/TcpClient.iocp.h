#ifndef _TCPCLIENT_IOCP_H_
#define _TCPCLIENT_IOCP_H_

#include "SocketBase.iocp.h"

namespace Network
{
	class TcpClient:
		public SocketBase
	{
		friend class SocketServer;
	public:
		TcpClient(int fd);
		TcpClient(uint ip, ushort port);
		virtual ~TcpClient();
		void Send(const byte * data, size_t len);
		void PendClose();
	protected:
		virtual void OnRecv(byte * data, size_t len) {}
	private:
#define BUF_MAX 1024
		byte input_buf[BUF_MAX];
		std::vector<byte> output_buf;
		size_t output_offset;

		bool _pend_close;

		void ClearSendQueue();
		virtual void _DoRecv(DWORD);
		virtual void _DoSend(DWORD);
		virtual void _WaitAccept(int);
	};
}

#endif // _TCPCLIENT_IOCP_H_
