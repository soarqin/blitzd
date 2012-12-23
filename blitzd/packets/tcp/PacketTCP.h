#ifndef _PACKETTCP_H_
#define _PACKETTCP_H_

#include "utils/Stream.h"
#include "network/TcpClient.h"

namespace Packets
{
	namespace TCP
	{
		class PacketTCP
		{
		public:
			virtual ~PacketTCP() {}
			inline void BuildAndSendTo(Network::TcpClient& cl)
			{
				if(Build())
					cl.Send((const byte *)_packet, _packet.size());
			}
			inline void SendTo(Network::TcpClient& cl)
			{
				cl.Send((const byte *)_packet, _packet.size());
			}
			virtual bool Build() = 0;

		protected:
			Utils::Stream _packet;
		};
	}
}

#define DECLARE_PACKET_TCP_BEGIN(name) \
class name: \
	public ::Packets::TCP::PacketTCP \
{ \
public: \
	virtual bool Build(); \

#define DECLARE_PACKET_TCP_END() \
}; \

#endif // _PACKETTCP_H_
