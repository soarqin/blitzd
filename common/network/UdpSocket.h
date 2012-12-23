#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

#if defined(WIN32) && defined(USE_IOCP)

#include "UdpSocket.iocp.h"

#else

#include "SocketBase.h"
#include "utils/SharedPtr.h"

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
		struct _send_data_t
		{
			uint toip;
			ushort toport;
			std::vector<byte> buffer;
		};
		std::queue<_send_data_t> _packet_queue;
		virtual void OnRecv(uint ip, ushort port, const byte * buf, size_t len) = 0;

		virtual void _DoRecv();
		virtual void _DoSend();
	};
}

#endif

#endif // _UDPSOCKET_H_
