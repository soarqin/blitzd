#include "Config.h"
#include "NewsCache.h"

namespace Cache
{
	NewsCache newsCache;

	void NewsCache::Load()
	{
		Utils::FileStream fs;
		fs.Open(Core::cfg.GetCfgDir() + '/' + Core::cfg.GetMotdFile());
		uint size = (uint)fs.Size();
		_motd.resize(size);
		fs.Read(&_motd[0], size);
		fs.Close();
		LOG_DEBUG(("Loaded motd: %s", _motd.c_str()));

		std::string content;
		fs.Open(Core::cfg.GetCfgDir() + '/' + Core::cfg.GetNewsFile());
		size = (uint)fs.Size();
		content.resize(size);
		fs.Read(&content[0], size);

		uint i = 0;
		while(i + 1 < content.length() && (content[i] != '{' || content[i + 1] != '{'))
			++ i;
		if(i + 1 < content.length())
			i += 2;
		while(i + 1< content.length())
		{
			uint start = i;
			while(i + 1 < content.length() && (content[i] != '}' || content[i + 1] != '}'))
				++ i;
			if(i + 1 >= content.length())
				break;
			std::string date(content.begin() + start, content.begin() + i);
			i += 2;
			while(i < content.length() && (content[i] == '\r' || content[i] == '\n'))
				++ i;
			if(i >= content.length())
				break;
			start = i;
			while(i + 1 < content.length() && (content[i] != '{' || content[i + 1] != '{'))
				++ i;
			uint end = i;
			while(end > start && (content[end - 1] == '\r' || content[end - 1] == '\n'))
				-- end;
			Add(date, std::string(content.begin() + start, content.begin() + end));
			if(i + 1 < content.length())
				i += 2;
			else
				break;
		}
	}

	void NewsCache::Add( std::string t, std::string n )
	{
		LOG_TRACE(("Added news: %s %s", t.c_str(), n.c_str()));
		struct tm tm_;
		sscanf(t.c_str(), "%d-%d-%d %d:%d:%d", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
		tm_.tm_year -= 1900;
		tm_.tm_mon --;
		tm_.tm_isdst = -1;
		uint tt = (uint)mktime(&tm_);
		Add(tt, n);
	}

	uint NewsCache::OldestTime()
	{
		if(_newsmap.empty())
			return 0;
		return _newsmap.begin()->first;
	}

	uint NewsCache::NewestTime()
	{
		if(_newsmap.empty())
			return 0;
		return _newsmap.rbegin()->first;
	}

	byte NewsCache::GetAfter( uint t, std::map<uint, std::string>& result )
	{
		std::map<uint, std::string>::iterator it = _newsmap.upper_bound(t);
		if(it == _newsmap.end())
			return 0;
		++ it;
		byte c = 0;
		for(; it != _newsmap.end(); ++ it)
		{
			result[it->first] = it->second;
			++ c;
			if(c == 250)
				break;
		}
		return c;
	}
}
