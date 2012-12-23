#ifndef _PACKETUDP_H_
#define _PACKETUDP_H_

#include "utils/Stream.h"
#include "network/UdpSocket.h"

namespace Packets
{
	namespace UDP
	{
		class PacketUDP
		{
		public:
			virtual ~PacketUDP() {}
			inline void BuildAndSendTo(Network::UdpSocket& skt, uint ip, ushort port)
			{
				if(Build())
					skt.Send(ip, port, _packet);
			}
			inline void SendTo(Network::UdpSocket& skt, uint ip, ushort port)
			{
				skt.Send(ip, port, _packet);
			}
			virtual bool Build();

		protected:
			virtual bool Pack() = 0;
			virtual uint GetCmd() = 0;

		protected:
			Utils::Stream _packet;
		};
	}
}

#define DECLARE_PACKET_UDP_BEGIN(name, op) \
class name: \
	public PacketUDP\
{ \
protected: \
	virtual bool Pack(); \
	virtual uint GetCmd() {return op;} \
public: \

#define DECLARE_PACKET_UDP_END() \
}; \

#endif // _PACKETUDP_H_
