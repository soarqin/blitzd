#include "Config.h"
#include "Crc32.h"
#include "utils/File.h"
#include "utils/Memory.h"

namespace Crypto
{
	CRC32::CRC32()
	{
		static bool inited = false;
		if(inited)
		{
			return;
		}
		uint poly = 0xEDB88320;
	
		for(uint i = 0; i < 256; ++ i)
		{
			uint crc = i;
			for(uint j = 8; j > 0; -- j)
			{
				if(crc & 1)
					crc = (crc >> 1) ^ poly;
				else
					crc >>= 1;
			}
			crc32_table[i] = crc;
		}
		inited = true;
	}

	uint CRC32::CalcData( const byte * data, uint size, uint initial /*= 0xFFFFFFFF*/ )
	{
		uint crc = initial;
		while(size > 0)
		{
			crc = ((crc) >> 8) ^ crc32_table[(*data) ^ ((crc) & 0x000000FF)];
			-- size;
			++ data;
		}
		return ~crc;
	}

	uint CRC32::CalcFile( const char * filename )
	{
		Utils::FileStream fs;
		if(!fs.Open(filename))
			return 0;
		uint size = (uint)fs.Size();

		byte * data = (byte *)_MEM.Alloc(size);
		if(data == NULL)
		{
			fs.Close();
			return 0;
		}
		fs.Read(data, size);
		fs.Close();
		uint crc = CalcData(data, size);
		_MEM.Free(data);
		return crc;
	}

	uint CRC32::crc32_table[256];
}
