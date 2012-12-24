#include "Config.h"
#include "FileCache.h"
#include "core/Cfg.h"
#include "utils/Timestamp.h"
#include <sys/stat.h>
#include <sys/types.h>

namespace Cache
{
	FileCache fileCache;

	FileCacheItem::Pointer FileCacheItem::Load( std::string n )
	{
		Utils::FileStream fs;
		if(!fs.Open(n))
		{
			LOG_ERROR(("Cannot load file: %s", n.c_str()));
			return FileCacheItem::Pointer();
		}
		FileCacheItem::Pointer ptr(new FileCacheItem(fs));
		fs.Close();
#ifdef _WIN32
		HANDLE hFile = CreateFileA(n.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);
		if(hFile != INVALID_HANDLE_VALUE && hFile != NULL)
		{
			FILETIME ft;
			::GetFileTime(hFile, NULL, NULL, &ft);
			ptr->_ft = (uint64)ft.dwLowDateTime + ((uint64)ft.dwHighDateTime << 32);
			CloseHandle(hFile);
		}
		else
			ptr->_ft = 0;
#else
		struct stat st;
		if(stat(n.c_str(), &st) == 0)
			ptr->_ft = Utils::Timestamp::fromEpochTime(st.st_mtime).fileTime();
		else
			ptr->_ft = 0;
#endif
		return ptr;
	}

	std::vector<byte>& FileCacheItem::GetData()
	{
		return _data;
	}

	FileCacheItem::FileCacheItem( Utils::FileStream& fs )
	{
		uint size = (uint)fs.Size();
		_data.append_zero(size);
		fs.Read(&_data[0], size);
	}

	uint FileCacheItem::GetSize()
	{
		return _data.size();
	}

	uint64 FileCacheItem::GetFileTime()
	{
		return _ft;
	}

	FileCacheItem::Pointer FileCache::FetchFile( std::string& filename )
	{
		FileCacheItem::Pointer ptr = FileCacheItem::Load(Core::cfg.GetFilesDir() + '/' + filename);
		if(ptr.get() != NULL)
			_cache_map[filename] = ptr;
		return ptr;
	}

	FileCacheItem::Pointer FileCache::operator[]( std::string fn )
	{
		std::map<std::string, FileCacheItem::Pointer>::iterator it = _cache_map.find(fn);
		if(it == _cache_map.end())
		{
			return FetchFile(fn);
		}
		return it->second;
	}
}
