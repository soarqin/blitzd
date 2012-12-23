#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#if defined(WIN32) && defined(USE_IOCP)

#include "TcpClient.iocp.h"

#else

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
		virtual void OnRecv(byte * data, size_t len) {}
	private:
#pragma pack(push, 4)
		typedef struct
#ifdef __GNUC__
			__attribute__((aligned(4)))
#endif
		{
			uint len;
			byte buf[1];
		} _packet_t;
#pragma pack(pop)
		bool _pend_close;
		size_t _send_pos;
		std::queue<_packet_t *> _send_queue;

		void ClearSendQueue();
		virtual void _DoRecv();
		virtual void _DoSend();
	};
}

#endif

#endif // _TCPCLIENT_H_
