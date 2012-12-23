#include "Config.h"
#include "GameResult.h"

namespace Plugins
{
	namespace BNet
	{
		bool GameResult::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			uint gtype, rcount;
			in >> gtype >> rcount;
			if(rcount > 12)
				return false;
			std::vector<uint> gresult;
			std::vector<std::string> pname;
			gresult.resize(rcount);
			pname.resize(rcount);
			for(uint i = 0; i < rcount; ++ i)
			{
				in >> gresult[i] >> pname[i];
			}
			std::string mapname, gamescores;
			in >> mapname >> gamescores;
			return true;
		}
	}
}
