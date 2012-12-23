#ifndef _PACKEDFILE_H_
#define _PACKEDFILE_H_

#include "utils/Stream.h"

namespace HostBot
{
	class PackedFile
	{
	public:
		virtual bool Load(const char * filename, bool = false);
		virtual bool Save(const char * filename);

		inline uint GetGameLength() {return _gameLength;}
		inline uint GetGameVersion() {return _gameVersion;}
	protected:
		Utils::Stream _data;
		uint _gameLength;
		uint _gameVersion;
	};
}

#endif // _PACKEDFILE_H_
