#ifndef _CHECKREVISION_H_
#define _CHECKREVISION_H_

#include "utils/Singleton.h"

namespace Cache
{
	class CheckRevision:
		public Utils::Singleton<CheckRevision>
	{
	public:
		struct Revision
		{
			std::string gameName;
			uint gameId;
			uint chksum[8];
			uint exeVer;
		};
	public:
		void Load();
		Revision* findRev(int verIndex, uint chksum, uint exeVer);
		const char * GetFormula();
	private:
		std::string _formula;
		std::vector<Revision> _revlist;
	private:
		static bool Calculate(std::string& formulaStr, std::string * fileNames, uint * result, uint sindex = 0, uint eindex = 7);
	};
}

#endif // _CHECKREVISION_H_

