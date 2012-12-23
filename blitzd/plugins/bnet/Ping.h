#ifndef _PING_H_
#define _PING_H_

#include "interface/IPlugin.h"

namespace Plugins
{
	namespace BNet
	{
		DECLARE_TCP_PLUGIN(Ping, 0x25FF)
	}
}

#endif // _PING_H_
