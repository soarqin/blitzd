#include "Config.h"
#include "ChannelCache.h"

#include "core/Client.h"
#include "core/Cfg.h"
#include "core/Message.h"
#include "utils/Memory.h"
#include "utils/DataConv.h"
#include "text/XmlNode.h"

namespace Cache
{
	ChannelCache channelCache;

	ChannelCache::ChannelCache()
	{
		_channelmap.rehash(Core::cfg.GetHashtableSize());
	}

	ChannelCacheItem::ChannelCacheItem( std::string shortname, std::string name, uint gameid, uint flag /*= Public*/, uint maxUser /*= 100*/, bool byCountry /*= false*/, bool byIndex /*= false*/ )
	{
		_short = shortname;
		_name = name;
		_gameid = gameid;
		_flag = flag;
		_maxUser = maxUser;
		_byCountry = byCountry;
		_byIndex = byIndex;
		_spawned = false;
	}

	ChannelCacheItem * ChannelCacheItem::Spawn( std::string name )
	{
		ChannelCacheItem * result = new ChannelCacheItem(_short, name, _gameid, _flag, _maxUser, false, false);
		result->_spawned = true;
		return result;
	}

	void ChannelCacheItem::Join( Core::Client* conn )
	{
		if(conn->GetGame() != NULL)
		{
			conn->GetGame()->Leave(conn);
		}
		if(conn->GetChannel() != NULL)
		{
			conn->GetChannel()->Leave(conn);
		}
		conn->SetChannel(this);

		_connset.insert(conn);
		Core::Message::Send(conn, NULL, Core::Message::Channel, _name);
		Core::Message::Send(_connset, conn, Core::Message::Join, "");
		std::set<Core::Client *>::iterator it;
		for(it = _connset.begin(); it != _connset.end(); ++ it)
		{
			Core::Message::Send(conn, *it, Core::Message::Join, "");
		}
	}

	void ChannelCacheItem::Leave( Core::Client* conn )
	{
		_connset.erase(conn);
		Core::Message::Send(_connset, conn, Core::Message::Leave, "");
	}

	void ChannelCacheItem::Broadcast( std::string msg )
	{
		Core::Message::Send(_connset, NULL, Core::Message::Info, msg);
	}

	void ChannelCacheItem::Emote( Core::Client* c, std::string msg )
	{
		Core::Message::Send(_connset, c, Core::Message::Emote, msg);
	}

	void ChannelCacheItem::Talk( Core::Client* c, std::string msg )
	{
		Core::Message::Send(_connset, c, Core::Message::Talk, msg);
	}

	void ChannelCache::Load()
	{
		Text::XmlNode xml((Core::cfg.GetCfgDir() + '/' + Core::cfg.GetChannelFile()).c_str());

		std::auto_ptr<Text::XmlNode> node(xml.GetChild("Channels"));
		if(node.get() != NULL)
		{
			std::vector<std::string> keys;
			keys.push_back("name");
			keys.push_back("short");
			keys.push_back("gameId");
			keys.push_back("maxUser");
			keys.push_back("byCountry");
			keys.push_back("byIndex");
			keys.push_back("flag");

			std::vector<std::vector<std::string> > result;
			node->ReadArray(result, keys, "Channel");
			for(size_t i = 0; i < result.size(); ++ i)
			{
				std::string chname = result[i][0];
				if(chname.empty())
					continue;
				std::string shortname = result[i][1];
				std::string gid = result[i][2];
				uint gameid = 0;
				if(gid.length() >= 4)
				{
					gameid = Utils::Reverse(*(uint*)&gid[0]);
				}
				uint maxUser = (uint)strtoul(result[i][3].c_str(), NULL, 10);
				if(maxUser == 0)
					maxUser = (uint)-1;
				bool byCountry = strtoul(result[i][4].c_str(), NULL, 10) > 0;
				bool byIndex = strtoul(result[i][5].c_str(), NULL, 10) > 0;
				uint flag = (uint)strtoul(result[i][6].c_str(), NULL, 10);
				if(flag == 0)
					flag = ChannelCacheItem::Public;
				if(shortname.empty())
					shortname = chname;
				LOG_TRACE(("adding channel: %s %s", chname.c_str(), shortname.c_str()));
				Add(shortname, chname, gameid, flag, maxUser, byCountry, byIndex);
			}
		}
	}

	ChannelCacheItem::Pointer ChannelCache::operator[]( std::string name )
	{
		ChannelNameMap_t::iterator it = _channelmap.find(name);
		if(it == _channelmap.end())
			return ChannelCacheItem::Pointer();
		return it->second;
	}

	std::list<ChannelCacheItem::Pointer>& ChannelCache::operator[]( uint tag )
	{
		return _game_channelmap[tag];
	}

	void ChannelCache::Add( ChannelCacheItem::Pointer ch )
	{
		if(ch.get() == NULL)
			return;
		_channelmap[ch->GetShortName()] = ch;
		if(_STR.strcmpi(ch->GetShortName().c_str(), ch->GetName().c_str()) != 0)
			_channelmap[ch->GetName()] = ch;
		_game_channelmap[ch->GetGameId()].push_back(ch);
	}

	ChannelCacheItem::Pointer ChannelCache::Add( std::string shortname, std::string name, uint tag, uint flag /*= 0*/, uint maxUser /*= 100*/, bool byCountry /*= false*/, bool byIndex /*= false*/ )
	{
		ChannelCacheItem::Pointer result(new ChannelCacheItem(shortname, name, tag, flag, maxUser, byCountry, byIndex));
		Add(result);
		return result;
	}

	ChannelCacheItem::Pointer ChannelCache::Ensure( std::string name, uint tag, std::string country /*= ""*/ )
	{
		ChannelCacheItem::Pointer result = (*this)[name];
		if(result.get() == NULL)
			return Add(name, name, tag);
		std::string newname;
		if(result->IsByCountry())
		{
			if(newname.length() == 0)
				newname = result->GetName();
			newname = newname + ' ' + country;
		}
		if(result->IsByIndex())
		{
			std::string newname2;
			if(newname.length() == 0)
				newname = result->GetName();
			int idx = 1;
			while(1)
			{
				newname2 = newname + '-';
				Utils::NumberFormatter::append(newname2, idx);
				ChannelCacheItem::Pointer result2 = (*this)[newname2];
				if(result2.get() == NULL)
				{
					result2 = result->Spawn(newname2);
					Add(result2);
					return result2;
				}
				else if(result2->GetUserCount() < result2->GetMaxUser())
				{
					return result2;
				}
				++ idx;
			}
		}
		if(newname.length() > 0)
		{
			ChannelCacheItem::Pointer result2 = (*this)[newname];
			if(result2.get() == NULL)
			{
				result2 = result->Spawn(newname);
				Add(result2);
				return result2;
			}
			else if(result2->GetUserCount() < result2->GetMaxUser())
			{
				return result2;
			}
			return NULL;
		}
		if(result->GetUserCount() < result->GetMaxUser())
			return result;
		return ChannelCacheItem::Pointer();
	}

	void ChannelCache::Remove( std::string name )
	{
		ChannelNameMap_t::iterator it = _channelmap.find(name);
		if(it == _channelmap.end())
			return;

		std::list<ChannelCacheItem::Pointer>& list = _game_channelmap[it->second->GetGameId()];
		std::list<ChannelCacheItem::Pointer>::iterator it2;
		for(it2 = list.begin(); it2 != list.end(); ++ it2)
		{
			if(*it2 == it->second)
			{
				list.erase(it2);
				break;
			}
		}
		_channelmap.erase(it);
	}

	void ChannelCache::Remove( ChannelCacheItem::Pointer ch )
	{
		if(ch.get() == NULL)
			return;
		Remove(ch->GetName());
	}
}
