#ifndef _PACKETBAAL_H_
#define _PACKETBAAL_H_

#include "../tcp/PacketTCP.h"

namespace Packets
{
	namespace Baal
	{
		class PacketBaal:
			public TCP::PacketTCP
		{
		public:
			PacketBaal(uint);
			virtual bool Build();
		protected:
			virtual bool Pack() = 0;
			virtual ushort GetCmd() = 0;
		private:
			uint _token;
		};
	}
}

#define DECLARE_PACKET_BAAL_BEGIN(name, op) \
class name: \
	public PacketBaal \
{ \
public: \
	name(uint t): PacketBaal(t) {} \
protected: \
	virtual bool Pack(); \
	virtual ushort GetCmd() {return op;} \
public: \

#define DECLARE_PACKET_BAAL_END() \
}; \

#endif // _PACKETBAAL_H_
