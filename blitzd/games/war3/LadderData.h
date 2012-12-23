#ifndef _LADDERDATA_H_
#define _LADDERDATA_H_

#include "utils/Singleton.h"

namespace Games
{
	namespace War3
	{
		class LadderData:
			public Utils::Singleton<LadderData>
		{
		public:
			LadderData();
			void Load();
			std::vector<byte> GetUrlData();
			std::vector<byte> GetMapName();
			std::vector<byte> GetMapInfo();
			std::vector<byte> GetMapDesc();
			std::vector<byte> GetLadderData();
		private:
			std::vector<std::string> _maplist;
			std::vector<std::vector<byte> > _mapindex;

			std::string map_desc[18];
			std::string map_longDesc[18];
			std::string ladder_desc[10];
			std::string ladder_url[10];
			std::string page_url[4];

			byte mapinfo_prefix[18][5];

			void __AddMap(std::string mapname, byte type);
		};
	}
}

#endif // _LADDERDATA_H_
