#ifndef _CACHEFUNC_H_
#define _CACHEFUNC_H_

#include <cctype>
#include "utils/Memory.h"

namespace Cache
{
	class LowerCaseHash
	{
	public:
		size_t operator() (const std::string& s) const
		{
			size_t r = 5181;
			size_t l = s.length();
			const char * p = s.c_str();
			for(size_t i = 0; i < l; ++ i, ++ p)
			{
				r = (r << 5) ^ std::tolower(*p);
			}
			return r;
		}
	};
	class CompareInCaseLess
	{
	public:
		bool operator() (const std::string& s1, const std::string& s2) const
		{
			return _STR.strcmpi(s1.c_str(), s2.c_str()) < 0;
		}
	};
	class CompareInCaseEqual
	{
	public:
		bool operator() (const std::string& s1, const std::string& s2) const
		{
			return _STR.strcmpi(s1.c_str(), s2.c_str()) == 0;
		}
	};
}

#endif // _CACHEFUNC_H_
