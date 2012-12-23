#ifndef _UDPSOCKET_IOCP_H_
#define _UDPSOCKET_IOCP_H_

#include "SocketBase.iocp.h"
#include "utils/SharedPtr.h"
#include "utils/Mutex.h"

namespace Network
{
	class UdpSocket:
		public SocketBase,
		public Utils::SharedClass<UdpSocket>
	{
	public:
		UdpSocket();
		~UdpSocket();
		bool Init(ushort port, SocketServer * host);
		void Broadcast(ushort port, std::vector<byte>& buffer);
		void Send(uint ip, ushort port, std::vector<byte>& buffer);
	protected:
		byte input_buf[1536];

		struct _send_data_t
		{
			SOCKADDR_IN toaddr;
			std::vector<byte> buffer;
		};
		std::list<_send_data_t> _packet_queue;

		Utils::FastMutex _pq_mutex;

		virtual void OnRecv(uint ip, ushort port, const byte * buf, size_t len) = 0;

		virtual void _DoRecv(DWORD);
		virtual void _DoSend(DWORD);
	};
}

#endif // _UDPSOCKET_IOCP_H_
