#ifndef _UDPSERVERPING_H_
#define _UDPSERVERPING_H_

#include "PacketUDP.h"
namespace Packets
{
	namespace UDP
	{
		DECLARE_PACKET_UDP_BEGIN(UDPServerPing, 5)
		DECLARE_PACKET_UDP_END()
	}
}

#endif // _UDPSERVERPING_H_
