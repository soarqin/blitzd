#ifndef _REALMSTARTUPCONN_H_
#define _REALMSTARTUPCONN_H_

#include "PacketRealm.h"

namespace Packets
{
	namespace Realm
	{
		DECLARE_PACKET_REALM_BEGIN(RealmStartupConn, 0x01)
			uint result;
		DECLARE_PACKET_REALM_END()
	}
}

#endif // _REALMSTARTUPCONN_H_
