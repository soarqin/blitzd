#ifndef _CHANNELCACHE_H_
#define _CHANNELCACHE_H_

#include "utils/SharedPtr.h"
#include "utils/NumberFormatter.h"
#include "CacheFunc.h"

namespace Core
{
	class Client;
}

namespace Cache
{
	class ChannelCacheItem:
		public Utils::SharedClass<ChannelCacheItem>
	{
	public:
		enum
		{
			None = 0x00,
			Public = 0x01,
			Moderated = 0x02,
			Restricted = 0x04,
			TheVoid = 0x08,
			System = 0x20,
			Official = 0x1000
		};
		ChannelCacheItem(std::string shortname, std::string name, uint gameid, uint flag = Public, uint maxUser = 100, bool byCountry = false, bool byIndex = false);
		ChannelCacheItem * Spawn(std::string name);
		void Join(Core::Client* conn);
		void Leave(Core::Client* conn);
		inline std::string GetName() { return _name; }
		inline std::string GetShortName() { return _short; }
		inline uint GetUserCount() { return _connset.size(); }
		inline uint GetFlag() { return _flag; }
		inline void SetFlag(uint f) { _flag = f; }
		inline uint GetGameId() { return _gameid; }
		inline uint GetMaxUser() { return _maxUser; }
		inline void SetMaxUser(uint u) { _maxUser = u; }
		inline bool IsByCountry() { return _byCountry; }
		inline void SetByCountry(bool b) { _byCountry = b; }
		inline bool IsByIndex(){ return _byIndex; }
		inline void SetByIndex(bool b) { _byIndex = b; }
		inline bool IsSpawned() { return _spawned;}
		void Broadcast(std::string msg);
		void Emote(Core::Client* c, std::string msg);
		void Talk(Core::Client* c, std::string msg);

	private:
		std::set<Core::Client *> _connset;
		std::string _name;
		std::string _short;
		uint _gameid;
		uint _flag;
		uint _maxUser;
		bool _byCountry, _byIndex, _spawned;
	};

	class ChannelCache
	{
	public:
		ChannelCache();
		void Load();
		ChannelCacheItem::Pointer operator[](std::string name);
		std::list<ChannelCacheItem::Pointer>& operator[](uint tag);
		void Add(ChannelCacheItem::Pointer ch);
		ChannelCacheItem::Pointer Add(std::string shortname, std::string name, uint tag, uint flag = 0, uint maxUser = 100, bool byCountry = false, bool byIndex = false);
		ChannelCacheItem::Pointer Ensure(std::string name, uint tag, std::string country = "");
		void Remove(std::string name);
		void Remove(ChannelCacheItem::Pointer ch);
	private:
		typedef std::unordered_map<std::string, ChannelCacheItem::Pointer, LowerCaseHash, CompareInCaseEqual> ChannelNameMap_t;
		typedef std::map<uint, std::list<ChannelCacheItem::Pointer> > ChannelGameMap_t;

		ChannelNameMap_t _channelmap;
		ChannelGameMap_t _game_channelmap;
	};

	extern ChannelCache channelCache;
}

#endif // _CHANNELCACHE_H_
