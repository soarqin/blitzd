#include "Config.h"
#include <zlib.h>
#include "PackedFile.h"
#include "utils/File.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "../zlib/zlibd.lib")
#else
#pragma comment(lib, "../zlib/zlib.lib")
#endif
#endif

namespace HostBot
{
	int tzuncompress( Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen )
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

		err = inflateInit(&stream);
		if (err != Z_OK) return err;

		err = inflate(&stream, Z_SYNC_FLUSH);
		if (err != Z_STREAM_END && err != Z_OK) {
			inflateEnd(&stream);
			if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
				return Z_DATA_ERROR;
			return err;
		}
		*destLen = stream.total_out;

		err = inflateEnd(&stream);
		return err;
	}

	bool PackedFile::Load( const char * filename, bool onlyFirst )
	{
		Utils::FileStream fs;
		fs.Open(filename);
		uint size = (uint)fs.Size();

		Utils::Stream st;
		st.append_zero(size);
		fs.Read((byte *)st, size);
		fs.Close();

		_data.clear();

		std::string magicStr;
		st>>magicStr;
		if( magicStr != "Warcraft III recorded game\x01A" )
		{
			return false;
		}

		uint HeaderVersion;
		uint DecompressedSize;
		uint NumBlocks;
		st += 4;						// header size
		st += 4;						// compressed file size
		st >> HeaderVersion;			// header version
		st >> DecompressedSize;			// decompressed file size
		st >> NumBlocks;				// number of blocks

		if( HeaderVersion == 0 )
		{
			st += 2;					// unknown
			st >> (ushort&)_gameVersion;	// version number
		}
		else
		{
			st += 4;					// version identifier
			st >> _gameVersion;			// version number
		}

		st += 2;						// build number
		st += 2;						// flags
		st >> _gameLength;				// game length
		st += 4;						// CRC

		uint n = NumBlocks;
		if(onlyFirst)
			n = 1;
		for(uint i = 0; i < n; ++ i)
		{
			ushort comp, decomp;
			st >> comp >> decomp;
			st += 4;
			std::vector<byte> blockcomp, blockdecomp;
			blockcomp.resize(comp);
			blockdecomp.resize(decomp);
			st >> blockcomp;
			uLongf destlen = decomp;
			if(tzuncompress(&blockdecomp[0], &destlen, &blockcomp[0], comp) != Z_OK || destlen != decomp)
			{
				_data.clear();
				return false;
			}
			_data<<blockdecomp;
		}
		fs.Open("D:\\123.bin", true);
		fs.Write((byte *)_data, _data.size());
		fs.Close();

		return true;
	}

	bool PackedFile::Save( const char * filename )
	{
		return true;
	}
}
