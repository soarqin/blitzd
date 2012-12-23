#include "Config.h"
#include "RealmCache.h"

namespace Cache
{
	RealmCache realmCache;

	RealmCacheItem::RealmCacheItem( std::string name, std::string desc, ushort port, uint ip): _name(name), _desc(desc), _port(port), _ip(ip)
	{
	}

	RealmCacheItem::Pointer RealmCache::Add( std::string name, std::string desc, ushort port, uint ip )
	{
		RealmMap_t::iterator it = _realmMap.find(name);
		if(it != _realmMap.end())
			return RealmCacheItem::Pointer();
		RealmCacheItem::Pointer ptr(new RealmCacheItem(name, desc, port, ip));
		_realmMap[name] = ptr;
		return ptr;
	}

	RealmCacheItem::Pointer RealmCache::operator[]( std::string name )
	{
		RealmMap_t::iterator it = _realmMap.find(name);
		if(it == _realmMap.end())
			return RealmCacheItem::Pointer();
		return it->second;
	}

	void RealmCache::BuildPacket( Utils::Stream& out )
	{
		out << (uint)_realmMap.size();
		RealmMap_t::iterator it;
		for(it = _realmMap.begin(); it != _realmMap.end(); ++ it)
		{
			out << (uint)1 << it->second->GetName() << it->second->GetDescription();
		}
	}
}
