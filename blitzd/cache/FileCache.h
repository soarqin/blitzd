#ifndef _FILECACHE_H_
#define _FILECACHE_H_

#include "utils/SharedPtr.h"
#include "utils/Stream.h"
#include "utils/File.h"

namespace Cache
{
	class FileCacheItem:
		public Utils::SharedClass<FileCacheItem>
	{
	public:
		static FileCacheItem::Pointer Load(std::string);
		std::vector<byte>& GetData();
		uint GetSize();
		uint64 GetFileTime();
	private:
		FileCacheItem(Utils::FileStream&);
		Utils::Stream _data;
		uint64 _ft;
	};

	class FileCache
	{
	public:
		FileCacheItem::Pointer operator[] (std::string);
	private:
		std::map<std::string, FileCacheItem::Pointer> _cache_map;

		FileCacheItem::Pointer FetchFile(std::string&);
	};

	extern FileCache fileCache;
}

#endif // _FILECACHE_H_
