#include "Config.h"
#include "FileToken.h"

namespace Packets
{
	namespace File
	{
		bool FileToken::Build()
		{
			_packet << token;
			return true;
		}
	}
}
