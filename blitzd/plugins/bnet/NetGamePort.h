#ifndef _NETGAMEPORT_H_
#define _NETGAMEPORT_H_

#include "interface/IPlugin.h"

namespace Plugins
{
	namespace BNet
	{
		DECLARE_TCP_PLUGIN(NetGamePort, 0x45FF)
	}
}

#endif // _NETGAMEPORT_H_
