#include "Config.h"
#include "BNetReadUserData.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetReadUserData::Pack()
		{
			_packet << acount << kcount << token;
			for(uint i = 0; i < acount; ++ i)
				for(uint j = 0; j < kcount; ++ j)
				{
					_packet << result[i][j];
				}
			return true;
		}
	}
}
