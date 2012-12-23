#ifndef _PACKETREALM_H_
#define _PACKETREALM_H_

#include "../tcp/PacketTCP.h"

namespace Packets
{
	namespace Realm
	{
		class PacketRealm:
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

#define DECLARE_PACKET_REALM_BEGIN(name, op) \
class name: \
	public PacketRealm \
{ \
protected: \
	virtual bool Pack(); \
	virtual ushort GetCmd() {return op;} \
public: \

#define DECLARE_PACKET_REALM_END() \
}; \

#endif // _PACKETREALM_H_
