#include "Config.h"
#include "BlitzConn.h"
#include "Cfg.h"
#include "RealmTCPServer.h"
#include "network/Network.h"
#include "plugins/baal/PluginBaal.h"

namespace Core
{
	BlitzConn * BlitzConn::_conn = NULL;

	BlitzConn::BlitzConn(): Network::TcpClient(Network::ResolveAddress(cfg.GetBlitzDAddr().c_str()), cfg.GetBlitzDPort())
	{
		_Register(&RealmTCPServer::GetSingleton(), false);

		byte p = 0x65;
		Send(&p, 1);
	}

	BlitzConn::~BlitzConn()
	{
	}

	void BlitzConn::OnRecv( byte * buf, size_t len )
	{
		_buffer.append(buf, len);
		ProcessBaal();
	}

	void BlitzConn::ProcessBaal()
	{
		size_t size;
		ushort psize;
		while((size = _buffer.size()) >= 4 && (psize = *(ushort *)&_buffer[0]) <= size)
		{
			byte * buf = _buffer;
			Utils::Stream st(buf + 4, psize - 4);
			Plugins::Baal::PluginBaal::GetSingleton().Process(*this, *(ushort *)(buf + 2), st);
			if(psize < size)
				_buffer.pop_front(psize);
			else
				_buffer.clear();
		}
	}

	void BlitzConn::Init()
	{
		try
		{
			_conn = new BlitzConn;
		}
		catch(...)
		{
			_conn = NULL;
			LOG_INFO(("Unable to establish connection, please check up your network and config file!"));
		}
	}

	BlitzConn& BlitzConn::Get()
	{
		return *_conn;
	}
}
