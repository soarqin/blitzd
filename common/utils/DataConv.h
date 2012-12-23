#ifndef _DATACONV_H_
#define _DATACONV_H_

namespace Utils
{
	inline uint Reverse(uint n)
	{
		return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
	}
}

#endif // _DATACONV_H_
