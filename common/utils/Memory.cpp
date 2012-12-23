#include "Config.h"
#include "Memory.h"
#ifdef WIN32
#include <shlwapi.h>
#else
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#endif

namespace Utils
{
#ifdef WIN32
	static HANDLE _Heap = GetProcessHeap();
	inline void * win32_alloc(int size)
	{
		return HeapAlloc(_Heap, 0, size);
	}

	inline void * win32_calloc(int size)
	{
		return HeapAlloc(_Heap, HEAP_ZERO_MEMORY, size);
	}

	inline void * win32_realloc(void * buf, int size)
	{
		return HeapReAlloc(_Heap, 0, buf, size);
	}

	inline void win32_free(void * buf)
	{
		HeapFree(_Heap, 0, buf);
	}

	inline int win32_strlena(const char * str)
	{
		return lstrlenA(str);
	}

	inline void win32_strcpya(char * dst, const char * src)
	{
		lstrcpyA(dst, src);
	}

	inline void win32_strcpyna(char * dst, const char * src, int size)
	{
		lstrcpynA(dst, src, size);
	}

	inline int win32_strcmpa(const char * s1, const char * s2)
	{
		return lstrcmpA(s1, s2);
	}

	inline int win32_strcmpia(const char * s1, const char * s2)
	{
		return lstrcmpiA(s1, s2);
	}

	inline int win32_strcmpna(const char * s1, const char * s2, int size)
	{
		return StrCmpNA(s1, s2, size);
	}

	inline int win32_strcmpnia(const char * s1, const char * s2, int size)
	{
		return StrCmpNIA(s1, s2, size);
	}

	inline char * win32_strdupa(const char * s)
	{
		int len = win32_strlena(s);
		char * ns = (char *)_MEM.Alloc(len * 2 + 2);
		if(ns == NULL)
			return NULL;
		win32_strcpya(ns, s);
		return ns;
	}
#else
	inline void * generic_alloc(int size)
	{
		return malloc(size);
	}

	inline void * generic_calloc(int size)
	{
		return calloc(1, size);
	}

	inline void * generic_realloc(void * buf, int size)
	{
		return realloc(buf, size);
	}

	inline void generic_free(void * buf)
	{
		free(buf);
	}

	inline int generic_strlena(const char * str)
	{
		return strlen(str);
	}

	inline void generic_strcpya(char * dst, const char * src)
	{
		strcpy(dst, src);
	}

	inline void generic_strcpyna(char * dst, const char * src, int size)
	{
		strncpy(dst, src, size);
	}

	inline int generic_strcmpa(const char * s1, const char * s2)
	{
		return strcmp(s1, s2);
	}

	inline int generic_strcmpia(const char * s1, const char * s2)
	{
		return strcasecmp(s1, s2);
	}

	inline int generic_strcmpna(const char * s1, const char * s2, int size)
	{
		return strncmp(s1, s2, size);
	}

	inline int generic_strcmpnia(const char * s1, const char * s2, int size)
	{
		return strncasecmp(s1, s2, size);
	}

	inline char * generic_strdupa(const char * s)
	{
		int len = generic_strlena(s);
		char * ns = (char *)_MEM.Alloc(len * 2 + 2);
		if(ns == NULL)
			return NULL;
		generic_strcpya(ns, s);
		return ns;
	}

#endif
}
#ifdef WIN32
Utils::MEM_t _MEM = {
	Utils::win32_alloc,
	Utils::win32_calloc,
	Utils::win32_realloc,
	Utils::win32_free
};

Utils::STR_t _STR = {
	Utils::win32_strlena,
	Utils::win32_strcpya,
	Utils::win32_strcpyna,
	Utils::win32_strcmpa,
	Utils::win32_strcmpia,
	Utils::win32_strcmpna,
	Utils::win32_strcmpnia,
	Utils::win32_strdupa
};
#else
Utils::MEM_t _MEM = {
	Utils::generic_alloc,
	Utils::generic_calloc,
	Utils::generic_realloc,
	Utils::generic_free
};

Utils::STR_t _STR = {
	Utils::generic_strlena,
	Utils::generic_strcpya,
	Utils::generic_strcpyna,
	Utils::generic_strcmpa,
	Utils::generic_strcmpia,
	Utils::generic_strcmpna,
	Utils::generic_strcmpnia,
	Utils::generic_strdupa
};
#endif
