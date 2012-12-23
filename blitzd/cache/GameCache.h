#ifndef _GAME_H_
#define _GAME_H_

#include "utils/SharedPtr.h"
#include "CacheFunc.h"

namespace Core
{
	class Client;
}

namespace Cache
{
	class GameCacheItem:
		public Utils::SharedClass<GameCacheItem>
	{
	public:
		GameCacheItem(std::string name);
		~GameCacheItem();
		void UpdateStatus(uint f, uint e, uint t, uint o, uint l);
		inline std::string GetName() { return _name; }
		inline uint GetFlag() { return _flag; }
		inline uint GetElapsed() { return _stime; }
		inline uint GetType() { return _type; }
		inline uint GetOption() { return _option; }
		inline uint GetLadder() { return _ladder; }
		inline std::string GetPasswd() { return _passwd; }
		inline void SetPasswd(std::string pwd) { _passwd = pwd; }
		inline std::string GetStat() { return _stat; }
		inline void SetStat(std::string s) { _stat = s; }
		inline bool GetBroadcast() { return _broadcast; }
		inline void SetBroadcast(bool b) { _broadcast = b; }
		inline bool IsHost(Core::Client * conn) { return _host == conn; }
		inline Core::Client * GetHost() { return _host; }

		void Join(Core::Client * conn);
		void Leave(Core::Client * conn);
	private:
		std::string _name;
		uint _flag;
		uint _stime;
		uint _type;
		uint _option;
		uint _ladder;
		std::string _passwd;
		std::string _stat;
		bool _broadcast;
	
		Core::Client * _host;
		std::list<Core::Client *> _connlist;
	};

	class GameCache
	{
	public:
		typedef std::unordered_map<std::string, GameCacheItem::Pointer, LowerCaseHash, CompareInCaseEqual> GameMap_t;
	public:
		GameCache();
		GameCacheItem::Pointer Add(std::string name);
		void Remove(std::string name);
		GameCacheItem::Pointer operator[] (std::string name);
		inline uint Size() { return _game_map.size(); }
		inline GameMap_t& GetGames() {return _game_map;}
		inline uint GetCount() {return (uint)_game_map.size();}

	private:
		GameMap_t _game_map;
	};
	extern GameCache gameCache;
}

#endif // _GAME_H_
