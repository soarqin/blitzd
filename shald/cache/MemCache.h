#ifndef _MEMCACHE_H_
#define _MEMCACHE_H_

namespace Cache
{
	class MemCache
	{
	public:
		MemCache();
		~MemCache();
		bool Init(size_t memsize);
		void Uninit();

		void * Alloc(size_t);
		void Free(void *);

		void PrintDebugInfo();
	private:
		byte * _mem_base;
		size_t _size;
		size_t _first_free;
#ifdef WIN32
		ULONG_PTR _numberOfPages;
		ULONG_PTR *_aPFNs;
#endif
	};
}

#endif // _MEMCACHE_H_
