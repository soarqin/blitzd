#ifndef _REALMCACHE_H_
#define _REALMCACHE_H_

#include "utils/SharedPtr.h"
#include "utils/Stream.h"

namespace Cache
{
	class RealmCacheItem:
		public Utils::SharedClass<RealmCacheItem>
	{
	public:
		RealmCacheItem(std::string, std::string, ushort, uint);

		inline std::string GetName() {return _name;}
		inline std::string GetDescription() {return _desc;}
		inline ushort GetPort() {return _port;}
		inline uint GetIP() {return _ip;}
	private:
		std::string _name;
		std::string _desc;
		ushort _port;
		uint _ip;
	};

	class RealmCache
	{
	public:
		RealmCacheItem::Pointer Add(std::string, std::string, ushort, uint);
		RealmCacheItem::Pointer operator[](std::string);
		void BuildPacket(Utils::Stream&);
	private:
		typedef std::map<std::string, RealmCacheItem::Pointer> RealmMap_t;
		RealmMap_t _realmMap;
	};

	extern RealmCache realmCache;
}

#endif // _REALMCACHE_H_
