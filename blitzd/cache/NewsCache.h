#ifndef _NEWSCACHE_H_
#define _NEWSCACHE_H_

#include "utils/File.h"
#include "core/Cfg.h"

namespace Cache
{
	class NewsCache
	{
	public:
		void Load();
		inline void SetMotd(std::string m) { _motd = m; }
		inline std::string GetMotd() { return _motd; }
		inline void Add(uint t, std::string n) { _newsmap[t] = n; }
		void Add(std::string t, std::string n);
		uint OldestTime();
		uint NewestTime();
		byte GetAfter(uint t, std::map<uint, std::string>& result);
	private:
		std::map<uint, std::string> _newsmap;
		std::string _motd;
	};
	extern NewsCache newsCache;
}

#endif // _NEWSCACHE_H_
