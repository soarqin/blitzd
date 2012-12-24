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

	void Client::OnRecv( Network::SocketBuf& sbuf )
	{
		size_t len = sbuf.length();
		if(len == 0)
			return;
		if(_stage == CONNECTED)
		{
			byte op;
			sbuf.read(&op, 1);
			Utils::Stream st;
			Plugins::Init::PluginInit::GetSingleton().Process(*this, op, st);
			len = sbuf.length();
			if(len == 0)
				return;
		}
		switch(_stage)
		{
		case BNET:
			ProcessBNet(sbuf);
			break;
		case BNET_LOGGEDIN:
			ProcessBNetLoggedIn(sbuf);
			break;
		case FILETRANS:
		case FILERAW:
			ProcessFile(sbuf);
			break;
		case BAAL:
			ProcessBaal(sbuf);
			break;
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

	void Client::ProcessBNet(Network::SocketBuf& sbuf)
	{
		size_t size;
		while((size = sbuf.length()) >= 4)
		{
			uint hdr;
			sbuf.copyout(&hdr, 4);
			ushort psize = (ushort)((hdr & 0xFFFF0000) >> 16);
			if(psize > size)
				return;
			psize -= 4;
			sbuf.drain(4);
			byte * buf = new(std::nothrow) byte[psize];
			if(buf == NULL)
			{
				sbuf.drain(psize);
				continue;
			}
			sbuf.read(buf, psize);
			Utils::Stream st(buf, psize);
			Plugins::BNet::PluginBNet::GetSingleton().Process(*this, (ushort)(hdr & 0xFFFF), st);
			delete[] buf;
		}
	}

	void Client::ProcessBNetLoggedIn(Network::SocketBuf& sbuf)
	{
		size_t size;
		while((size = sbuf.length()) >= 4)
		{
			uint hdr;
			sbuf.copyout(&hdr, 4);
			ushort psize = (ushort)((hdr & 0xFFFF0000) >> 16);
			if(psize > size)
				return;
			psize -= 4;
			sbuf.drain(4);
			byte * buf = new(std::nothrow) byte[psize];
			if(buf == NULL)
			{
				sbuf.drain(psize);
				continue;
			}
			sbuf.read(buf, psize);
			Utils::Stream st(buf, psize);
			Plugins::BNet::PluginBNetLoggedIn::GetSingleton().Process(*this, (ushort)(hdr & 0xFFFF), st);
			delete[] buf;
		}
	}

	void Client::ProcessFile(Network::SocketBuf& sbuf)
	{
		while(1)
		{
			size_t size;
			switch(_stage)
			{
			case FILETRANS:
				if((size = sbuf.length()) >= 4)
				{
					uint hdr;
					sbuf.copyout(&hdr, 4);
					ushort psize = (ushort)(hdr & 0xFFFF);
					if(psize > size)
						return;
					sbuf.drain(4);
					psize -= 4;
					byte * buf = new(std::nothrow) byte[psize];
					if(buf == NULL)
					{
						sbuf.drain(psize);
						continue;
					}
					sbuf.read(buf, psize);
					Utils::Stream st(buf, psize);
					Plugins::File::PluginFile::GetSingleton().Process(*this, (ushort)((hdr & 0xFFFF0000) >> 16), st);
					delete[] buf;
				}
				else
					return;
				break;
			case FILERAW:
				{
					size = sbuf.length();
					byte * buf = new(std::nothrow) byte[size];
					if(buf == NULL)
					{
						sbuf.drain(size);
						continue;
					}
					sbuf.read(buf, size);
					Utils::Stream st(buf, size);
					Plugins::File::PluginFile::GetSingleton().Process(*this, 0, st);
					delete[] buf;
				}
				return;
            default:
                return;
			}
		}
	}

	void Client::ProcessBaal(Network::SocketBuf& sbuf)
	{
		size_t size;
		while((size = sbuf.length()) >= 4)
		{
			uint hdr;
			sbuf.copyout(&hdr, 4);
			ushort psize = (ushort)(hdr & 0xFFFF);
			if(psize > size)
				return;
			sbuf.drain(4);
			psize -= 4;
			byte * buf = new(std::nothrow) byte[psize];
			if(buf == NULL)
			{
				sbuf.drain(psize);
				continue;
			}
			sbuf.read(buf, psize);
			Utils::Stream st(buf, psize);
			Plugins::Baal::PluginBaal::GetSingleton().Process(*this, (ushort)((hdr & 0xFFFF0000) >> 16), st);
			delete[] buf;
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
