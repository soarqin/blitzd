#ifndef _PACKETBNET_H_
#define _PACKETBNET_H_

#include "../tcp/PacketTCP.h"

namespace Packets
{
	namespace BNet
	{
		class PacketBNet:
			public TCP::PacketTCP
		{
		public:
			virtual bool Build();
		protected:
			virtual bool Pack() = 0;
			virtual ushort GetCmd() = 0;
		};
	}
}

#define DECLARE_PACKET_BNET_BEGIN(name, op) \
class name: \
	public PacketBNet\
{ \
protected: \
	virtual bool Pack(); \
	virtual ushort GetCmd() {return op;} \
public: \

#define DECLARE_PACKET_BNET_END() \
}; \

#endif // _PACKETBNET_H_
