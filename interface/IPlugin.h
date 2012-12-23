#ifndef _IPLUGIN_H_
#define _IPLUGIN_H_

#include "utils/Singleton.h"
#include "utils/Stream.h"
#include "utils/SharedPtr.h"
#include "network/TcpClient.h"
#include "network/UdpSocket.h"

namespace Interface
{
	class IPlugin:
		public Utils::SharedClass<IPlugin>
	{
	public:
		virtual ushort GetOP() = 0;
		virtual bool Process(Network::TcpClient&, Utils::Stream&) = 0;
		virtual bool Process(Network::UdpSocket&, uint ip, ushort port, Utils::Stream&) = 0;
	};
	class TcpPlugin:
		public IPlugin
	{
	public:
		virtual bool Process(Network::UdpSocket&, uint ip, ushort port, Utils::Stream&) {return false;}
	};
	class UdpPlugin:
		public IPlugin
	{
	public:
		virtual bool Process(Network::TcpClient&, Utils::Stream&) {return false;}
	};
}

#define DECLARE_TCP_PLUGIN(name, id) \
	class name: \
		public Interface::TcpPlugin \
	{ \
	public: \
		virtual ushort GetOP() { return id; } \
		virtual bool Process(Network::TcpClient&, Utils::Stream&); \
	};

#define DECLARE_UDP_PLUGIN(name, id) \
	class name: \
		public Interface::UdpPlugin \
	{ \
	public: \
		virtual ushort GetOP() { return id; } \
		virtual bool Process(Network::UdpSocket&, uint, ushort, Utils::Stream&); \
	};

#endif // _IPLUGIN_H_
