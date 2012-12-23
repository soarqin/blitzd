#include "Config.h"
#include "LadderInfo.h"
#include "LadderData.h"

#include "utils/Stream.h"
#include <zlib.h>

#ifdef _MSC_VER
#pragma comment(lib, "../zlib/zlib.lib")
#endif

namespace Games
{
	namespace War3
	{
		static int tzcompress( Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen )
		{
			z_stream stream;
			int err;

			stream.next_in = (Bytef*)source;
			stream.avail_in = (uInt)sourceLen;

			stream.next_out = dest;
			stream.avail_out = (uInt)*destLen;
			if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

			stream.zalloc = (alloc_func)0;
			stream.zfree = (free_func)0;

			err = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
			if (err != Z_OK) return err;

			err = deflate(&stream, Z_FINISH);
			if (err != Z_STREAM_END) {
				deflateEnd(&stream);
				return err == Z_OK ? Z_BUF_ERROR : err;
			}
			*destLen = stream.total_out;

			err = deflateEnd(&stream);
			return err;
		}

		void LadderInfo::Init()
		{
			LadderData::GetSingleton().Load();
			_comp_urldata = BuildData(LadderData::GetSingleton().GetUrlData());
			_comp_mapname = BuildData(LadderData::GetSingleton().GetMapName());
			_comp_mapinfo = BuildData(LadderData::GetSingleton().GetMapInfo());
			_comp_mapdesc = BuildData(LadderData::GetSingleton().GetMapDesc());
			_comp_ladderdata = BuildData(LadderData::GetSingleton().GetLadderData());
		}

		std::vector<byte> LadderInfo::BuildData( std::vector<byte> data )
		{
			std::vector<byte> output;
			uLongf len = (uLongf)(data.size() + data.size() / 16);
			output.resize(4 + len);
			tzcompress(&output[4], &len, &data[0], data.size());
			output.resize(4 + len);
			*(ushort *)&output[0] = (ushort)data.size();
			*(ushort *)&output[2] = (ushort)len;
			return output;
		}
	}
}
