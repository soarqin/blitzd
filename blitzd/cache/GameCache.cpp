#include "Config.h"
#include "GameCache.h"
#include "core/Client.h"
#include "core/Cfg.h"

namespace Cache
{
	GameCache gameCache;
	GameCacheItem::GameCacheItem( std::string name )
	{
		_name = name;
		_host = NULL;
		_broadcast = true;
	}

	GameCacheItem::~GameCacheItem()
	{
		std::list<Core::Client *>::iterator it;
		for(it = _connlist.begin(); it != _connlist.end(); ++ it)
		{
			(*it)->SetGame(NULL);
		}
	}

	void GameCacheItem::UpdateStatus( uint f, uint e, uint t, uint o, uint l )
	{
		_flag = f;
		_stime = e;
		_type = t;
		_option = o;
		_ladder = l;
	}

	void GameCacheItem::Join( Core::Client * conn )
	{
		if(_connlist.empty())
		{
			_host = conn;
		}
		_connlist.push_back(conn);
		Cache::GameCacheItem * game = conn->GetGame();
		if(game != NULL)
		{
			game->Leave(conn);
		}
		Cache::ChannelCacheItem * channel = conn->GetChannel();
		if(channel != NULL)
		{
			channel->Leave(conn);
		}
		conn->SetGame(this);
	}

	void GameCacheItem::Leave( Core::Client * conn )
	{
		conn->SetGame(NULL);
		std::list<Core::Client *>::iterator it;
		for(it = _connlist.begin(); it != _connlist.end(); ++ it)
		{
			if(*it == conn)
			{
				_connlist.erase(it);
				break;
			}
		}
		if(_host == conn)
		{
			if(!_connlist.empty())
				_host = _connlist.front();
			else
				gameCache.Remove(_name);
		}
	}

	GameCache::GameCache()
	{
		_game_map.rehash(Core::cfg.GetHashtableSize());
	}

	GameCacheItem::Pointer GameCache::Add( std::string name )
	{
		GameCacheItem::Pointer ptr(new GameCacheItem(name));
		_game_map[name] = ptr;
		return ptr;
	}

	void GameCache::Remove( std::string name )
	{
		_game_map.erase(name);
	}

	GameCacheItem::Pointer GameCache::operator[]( std::string name )
	{
		GameMap_t::iterator it = _game_map.find(name);
		if(it != _game_map.end())
			return it->second;
		return GameCacheItem::Pointer();
	}
}
