#ifndef _LADDERINFO_H_
#define _LADDERINFO_H_

#include "utils/Singleton.h"

namespace Games
{
	namespace War3
	{
		class LadderInfo:
			public Utils::Singleton<LadderInfo>
		{
		public:
			void Init();
			inline std::vector<byte>& GetUrlData()
			{
				return _comp_urldata;
			}
			inline std::vector<byte>& GetMapName()
			{
				return _comp_mapname;
			}
			inline std::vector<byte>& GetMapInfo()
			{
				return _comp_mapinfo;
			}
			inline std::vector<byte>& GetMapDesc()
			{
				return _comp_mapdesc;
			}
			inline std::vector<byte>& GetLadderData()
			{
				return _comp_ladderdata;
			}

		private:
			std::vector<byte> _comp_urldata;
			std::vector<byte> _comp_mapname;
			std::vector<byte> _comp_mapinfo;
			std::vector<byte> _comp_mapdesc;
			std::vector<byte> _comp_ladderdata;

			std::vector<byte> BuildData(std::vector<byte> data);
		};
	}
}

#endif // _LADDERINFO_H_
