#include "Config.h"
#include "FileHeader.h"
#include "cache/FileCache.h"

namespace Packets
{
	namespace File
	{
		bool FileHeader::Build()
		{
			Cache::FileCacheItem::Pointer ptr = Cache::fileCache[fileName];
			if(ptr.get() == NULL)
				return false;
			std::vector<byte>& data = ptr->GetData();
			if(data.size() == 0 || offset >= data.size())
				return false;
			_packet << (uint)0 << (ptr->GetSize() - offset) << adid << extag << ptr->GetFileTime() << fileName.c_str();
			*(uint *)&_packet[0] = _packet.size();
			_packet.append(&data[offset], data.size() - offset);
			return true;
		}
	}
}
