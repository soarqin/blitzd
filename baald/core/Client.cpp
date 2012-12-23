#include "Config.h"
#include "Client.h"
#include "Cfg.h"
#include "plugins/init/PluginInit.h"
#include "plugins/realm/PluginRealm.h"

namespace Core
{
	ClientPool clientPool;

	static uint _current_session = 0;

	Client::Client( int fd ): Network::TcpClient(fd), _stage(CONNECTED), _session(++ _current_session)
	{
		clientPool.Add(this);
	}

	Client::~Client()
	{
		clientPool.Remove(_session);
	}

	void Client::OnRecv( byte * buf, size_t len )
	{
		if(len == 0)
			return;

		if(_stage == CONNECTED)
		{
			ushort op = buf[0];
			Utils::Stream st;
			Plugins::Init::PluginInit::GetSingleton().Process(*this, op, st);
			++ buf;
			-- len;
			if(len == 0)
				return;
		}
		_buffer.append(buf, len);
		switch(_stage)
		{
		case BNETREALM:
			ProcessBNetRealm();
			break;
		/*
		case D2GS:
			ProcessD2GS();
			break;
		*/
		default:
			break;
		}
	}

	void Client::ProcessBNetRealm()
	{
		size_t size;
		ushort psize;
		while((size = _buffer.size()) >= 3 && (psize = *(ushort *)&_buffer[0]) <= size)
		{
			byte * buf = _buffer;
			Utils::Stream st(buf + 3, psize - 3);
			Plugins::Realm::PluginRealm::GetSingleton().Process(*this, (ushort)buf[2], st);
			if(psize < size)
				_buffer.pop_front(psize);
			else
				_buffer.clear();
		}
	}

	ClientPool::ClientPool()
	{
		_sessionMap.rehash(cfg.GetHashtableSize());
	}

	void ClientPool::Remove( uint s )
	{
		_sessionMap.erase(s);
	}

	Client * ClientPool::operator[]( uint s )
	{
		SessionMap_t::iterator it = _sessionMap.find(s);
		if(it == _sessionMap.end())
			return NULL;
		return it->second;
	}

	void ClientPool::Add( Client * cl )
	{
		if(cl == NULL)
			return;
		_sessionMap[cl->GetSession()] = cl;
	}
}
