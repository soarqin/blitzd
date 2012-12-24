#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

#include "SocketBase.h"
#include "utils/SharedPtr.h"

struct event;

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
		virtual void OnRecv(uint ip, ushort port, const byte * buf, size_t len) = 0;

		virtual void DoRecv();
		virtual bool DoRegister();

	private:
		static void _client_read_cb(int, short, void *);;

	protected:
		struct event * _ev;
	};
}

#endif // _UDPSOCKET_H_
