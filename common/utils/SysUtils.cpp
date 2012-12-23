#include "Config.h"
#include "SysUtils.h"

namespace Utils
{
	uint GetNumberOfProcessors()
	{
#ifdef _WIN32
		static uint count = 0;
		if(count == 0)
		{
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			count = si.dwNumberOfProcessors;
		}
		return count;
#else
		return 1;
#endif
	}
}
