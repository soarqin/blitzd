#include "Config.h"
#include "Client.h"
#include "Cfg.h"

namespace Core
{
	ClientPool clientPool;

	static uint _current_session = 0;

	Client::Client( int fd ): Network::TcpClient(fd), _session(++ _current_session)
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
		Send(buf, len);
/*
		_buffer.append(buf, len);
		ProcessPacket();
		*/
	}

	void Client::ProcessPacket()
	{
		size_t size;
		ushort psize;
		while((size = _buffer.size()) >= 4 && (psize = *(ushort *)&_buffer[2]) <= size)
		{
			byte * buf = _buffer;
			Utils::Stream st(buf + 4, psize - 4);
//			Plugins::Realm::PluginRealm::GetSingleton().Process(*this, (ushort)buf[1], st);
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
