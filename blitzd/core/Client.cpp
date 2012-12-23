#include "Config.h"
#include "Client.h"
#include "Cfg.h"
#include "plugins/init/PluginInit.h"
#include "plugins/bnet/PluginBNet.h"
#include "plugins/bnet/PluginBNetLoggedIn.h"
#include "plugins/file/PluginFile.h"
#include "plugins/baal/PluginBaal.h"
#include "utils/Random.h"
#include "utils/SysTime.h"

#include "packets/bnet/BNetPing.h"

namespace Core
{
	ClientPool clientPool;

	static uint _current_session = 0;

	Client::Client( int fd ):
		Network::TcpClient(fd), _stage(CONNECTED), _platform(0), _gameid(0), _version(0),
		_gamelang(0), _natip(0), _tzone(0), _locale(0), _language(0), _gamePort(0), _check_sid(0), _checkTick(0), _latency(0),
		_protocolVer(0), _session(++ _current_session), _token(Utils::GetRandom32()), _flag(UF_NOUDP), _game(NULL), _channel(NULL)
	{
		struct sockaddr_in addr;
		socklen_t addrlen = sizeof(struct sockaddr_in);
		if(getpeername(fd, (struct sockaddr *)&addr, &addrlen) != SOCKET_ERROR)
		{
			_gameIp = ntohl(addr.sin_addr.s_addr);
		}
		else
		{
			_gameIp = 0;
		}
		clientPool.AddByToken(this);
	}

	Client::~Client()
	{
		if(_user.get() != NULL)
			clientPool.Remove(_user->GetUsername());
		else
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
		case BNET:
			ProcessBNet();
			break;
		case BNET_LOGGEDIN:
			ProcessBNetLoggedIn();
			break;
		case FILETRANS:
		case FILERAW:
			ProcessFile();
			break;
		case BAAL:
			ProcessBaal();
        default:
            break;
		}
	}

	void Client::SetAuthInfo( uint platform, uint gameid, uint ver, uint gamelang, uint natip, uint timezone, uint locale, uint language, std::string& countryab, std::string& country )
	{
		_platform = platform;
		_gameid = gameid;
		_version = ver;
		_gamelang = gamelang;
		_natip = natip;
		_tzone = timezone;
		_locale = locale;
		_language = language;
		_countryab = countryab;
		_country = country;
		if(_gameid == GAME_ID_W3XP || _gameid == GAME_ID_WAR3)
		{
			_protocolVer = 2;
		}
		else
		{
			_protocolVer = 0;
		}
	}

	void Client::ProcessBNet()
	{
		size_t size;
		ushort psize;
		while((size = _buffer.size()) >= 4 && (psize = *(ushort *)&_buffer[2]) <= size)
		{
			byte * buf = _buffer;
			Utils::Stream st(buf + 4, psize - 4);
			Plugins::BNet::PluginBNet::GetSingleton().Process(*this, *(ushort *)buf, st);
			if(psize < size)
				_buffer.pop_front(psize);
			else
				_buffer.clear();
		}
	}

	void Client::ProcessBNetLoggedIn()
	{
		size_t size;
		ushort psize;
		while((size = _buffer.size()) >= 4 && (psize = *(ushort *)&_buffer[2]) <= size)
		{
			byte * buf = _buffer;
			Utils::Stream st(buf + 4, psize - 4);
			Plugins::BNet::PluginBNetLoggedIn::GetSingleton().Process(*this, *(ushort *)buf, st);
			if(psize < size)
				_buffer.pop_front(psize);
			else
				_buffer.clear();
		}
	}

	void Client::ProcessFile()
	{
		while(1)
		{
			size_t size;
			ushort psize;
			const byte * buf = _buffer;
			switch(_stage)
			{
			case FILETRANS:
				if((size = _buffer.size()) >= 2 && (psize = *(ushort *)buf) <= size)
				{
					Utils::Stream st(buf + 4, psize - 4);
					Plugins::File::PluginFile::GetSingleton().Process(*this, *(ushort *)&buf[2], st);
					if(psize < size)
						_buffer.pop_front(psize);
					else
						_buffer.clear();
				}
				else
					return;
				break;
			case FILERAW:
				{
					Utils::Stream st(buf, _buffer.size());
					Plugins::File::PluginFile::GetSingleton().Process(*this, 0, st);
				}
				return;
            default:
                return;
			}
		}
	}

	void Client::ProcessBaal()
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

	void Client::SetUser( Cache::UserCacheItem::Pointer u )
	{
		if(_user.get() != NULL)
			clientPool.Remove(_user->GetUsername());
		_user = u;
		if(u.get() != NULL)
			clientPool.Add(this);
	}

	void Client::BuildStats()
	{
		_stats.resize(4);
		*(uint*)&_stats[0] = _gameid;
	}

	void Client::SetStage( Stage_t s )
	{
		_stage = s;
		if(s == BNET_LOGGEDIN)
		{
			BuildStats();
		}
	}

	void Client::SetGame( Cache::GameCacheItem * g )
	{
		_game = g;
	}

	void Client::SetChannel( Cache::ChannelCacheItem * ch )
	{
		_channel = ch;
	}

	void Client::CheckStart(uint sid)
	{
		_check_sid = sid;
		_checkTick = Utils::GetTicks();
	}

	void Client::CheckEnd(uint sid)
	{
		if(_check_sid != sid || _checkTick == 0)
			return;
		uint curTick = Utils::GetTicks();
		if(curTick < _checkTick)
		{
			_latency = 0;
		}
		else
		{
			_latency = curTick - _checkTick;
		}
	}

	void Client::SendPing()
	{
		Packets::BNet::BNetPing packet;
		uint sid = Utils::GetRandom32();
		CheckStart(sid);
		packet.sid = sid;
		packet.BuildAndSendTo(*this);
	}

	ClientPool::ClientPool()
	{
		_clientMap.rehash(cfg.GetHashtableSize());
		_tokenMap.rehash(cfg.GetHashtableSize());
	}

	void ClientPool::Add( Client * cl )
	{
		if(cl == NULL || cl->GetUser().get() == NULL)
			return;
		_clientMap[cl->GetUser()->GetUsername()] = cl;
	}

	void ClientPool::Remove( std::string name )
	{
		ClientMap_t::iterator it = _clientMap.find(name);
		if(it == _clientMap.end())
			return;
		_tokenMap.erase(it->second->GetToken());
		_clientMap.erase(it);
	}

	void ClientPool::Remove( uint s )
	{
		_tokenMap.erase(s);
	}

	Client * ClientPool::operator[]( std::string name )
	{
		ClientMap_t::iterator it = _clientMap.find(name);
		if(it == _clientMap.end())
			return NULL;
		return it->second;
	}

	Client * ClientPool::operator[]( uint s )
	{
		TokenMap_t::iterator it = _tokenMap.find(s);
		if(it == _tokenMap.end())
			return NULL;
		return it->second;
	}

	void ClientPool::AddByToken( Client * cl )
	{
		if(cl == NULL)
			return;
		_tokenMap[cl->GetToken()] = cl;
	}
}
