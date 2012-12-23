#include "Config.h"
#include "MemCache.h"
#include "utils/Exception.h"
#include "utils/Memory.h"

namespace Cache
{
#define ALIGN_BYTES (sizeof(size_t))
#define ALIGN_ALLOC (sizeof(size_t) * 2)
#define COUNT_ALIGNED(n) (((n) + (ALIGN_ALLOC - 1)) & ~(ALIGN_ALLOC - 1))
#define CHUNK_USED(n) (*(size_t *)(n))
#define CHUNK_NEXT(n) (*(size_t *)(n))
#define CHUNK_SIZE(n) (*(size_t *)((byte *)(n) + ALIGN_BYTES))
	MemCache::MemCache()
	{
#ifdef WIN32
		struct {
			DWORD Count;
			LUID_AND_ATTRIBUTES Privilege [1];
		} Info;

		HANDLE Token;
		BOOL Result;

		// Open the token.

		Result = OpenProcessToken ( GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES,
			& Token);

		if( Result != TRUE ) 
		{
			throw Utils::StringException( "Cannot open process token.\n" );
			return;
		}

		// Enable or disable?

		Info.Count = 1;
		Info.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Get the LUID.

		Result = LookupPrivilegeValue ( NULL,
			SE_LOCK_MEMORY_NAME,
			&(Info.Privilege[0].Luid));

		if( Result != TRUE ) 
		{
			throw Utils::StringException( "Cannot get privilege" );
			return;
		}

		// Adjust the privilege.

		Result = AdjustTokenPrivileges ( Token, FALSE,
			(PTOKEN_PRIVILEGES) &Info,
			0, NULL, NULL);

		// Check the result.

		if( Result != TRUE ) 
		{
			throw Utils::StringException("Cannot adjust token privileges\n");
			return;
		} 
		else 
		{
			if( GetLastError() != ERROR_SUCCESS ) 
			{
				throw Utils::StringException("Cannot enable the SE_LOCK_MEMORY_NAME privilege");
				return;
			}
		}

		CloseHandle( Token );
#endif
	}

	MemCache::~MemCache()
	{
		Uninit();
	}

	bool MemCache::Init( size_t memsize )
	{
		memsize = COUNT_ALIGNED(memsize);
		if(memsize == 0)
			return false;
#ifdef WIN32
		BOOL bResult;
		ULONG_PTR _numberOfPagesInitial;

		SYSTEM_INFO sSysInfo;
		int PFNArraySize;
		GetSystemInfo(&sSysInfo);

		_numberOfPages = (memsize + sSysInfo.dwPageSize - 1) / sSysInfo.dwPageSize;

		PFNArraySize = _numberOfPages * sizeof (ULONG_PTR);

		_aPFNs = (ULONG_PTR *) _MEM.Alloc(PFNArraySize);

		if (_aPFNs == NULL) 
		{
			return false;
		}

		// Allocate the physical memory.

		_numberOfPagesInitial = _numberOfPages;
		bResult = AllocateUserPhysicalPages( GetCurrentProcess(),
			&_numberOfPages,
			_aPFNs );

		if( bResult != TRUE ) 
		{
			_MEM.Free(_aPFNs);
			_aPFNs = NULL;
			return false;
		}

		if( _numberOfPagesInitial != _numberOfPages ) 
		{
			FreeUserPhysicalPages( GetCurrentProcess(), &_numberOfPages, _aPFNs );
			_MEM.Free(_aPFNs);
			_aPFNs = NULL;
			return false;
		}

		// Reserve the virtual memory.

		_mem_base = (byte *)VirtualAlloc( NULL, memsize, MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE );

		if( _mem_base == NULL ) 
		{
			FreeUserPhysicalPages( GetCurrentProcess(), &_numberOfPages, _aPFNs );
			_MEM.Free(_aPFNs);
			_aPFNs = NULL;
			return false;
		}

		// Map the physical memory into the window.

		bResult = MapUserPhysicalPages( (PVOID)_mem_base, _numberOfPages, _aPFNs );

		if( bResult != TRUE ) 
		{
			VirtualFree(_mem_base, 0, MEM_RELEASE);
			FreeUserPhysicalPages( GetCurrentProcess(), &_numberOfPages, _aPFNs );
			_MEM.Free(_aPFNs);
			_mem_base = NULL;
			_aPFNs = NULL;
			return false;
		}

#else
		_mem_base = _MEM.Alloc(memsize);
		if(_mem_base == NULL)
			return false;
#endif
		_size = memsize;
		_first_free = (size_t)_mem_base;
		CHUNK_NEXT(_mem_base) = 0;
		CHUNK_SIZE(_mem_base) = memsize;
		return true;
	}

	void MemCache::Uninit()
	{
		if(_mem_base == NULL)
			return;
#ifdef WIN32
		MapUserPhysicalPages( (PVOID)_mem_base, _numberOfPages, NULL );
		FreeUserPhysicalPages( GetCurrentProcess(), &_numberOfPages, _aPFNs );
		VirtualFree( _mem_base, 0, MEM_RELEASE );
		_MEM.Free(_aPFNs);
		_mem_base = NULL;
		_aPFNs = NULL;
#else
		_MEM.Free(_mem_base);
#endif
	}

	void * MemCache::Alloc( size_t memsize )
	{
		if(memsize == 0)
			return NULL;
		memsize = COUNT_ALIGNED(memsize + ALIGN_BYTES);
		size_t this_chunk = 0;
		size_t next_chunk = _first_free;
		while(next_chunk != 0)
		{
			size_t size = CHUNK_SIZE(next_chunk);
			if(size >= memsize)
			{
				size_t nnext_chunk = CHUNK_NEXT(next_chunk);
				if(size == memsize)
				{
					if(this_chunk == 0)
					{
						_first_free = nnext_chunk;
					}
					else
					{
						CHUNK_NEXT(this_chunk) = nnext_chunk;
					}
				}
				else
				{
					if(this_chunk == 0)
					{
						_first_free = next_chunk + memsize;
					}
					else
					{
						CHUNK_NEXT(this_chunk) = next_chunk + memsize;
					}
					CHUNK_NEXT(next_chunk + memsize) = nnext_chunk;
					CHUNK_SIZE(next_chunk + memsize) = size - memsize;
				}
				CHUNK_USED(next_chunk) = memsize;
				return (void *)(next_chunk + ALIGN_BYTES);
			}
			this_chunk = next_chunk;
			next_chunk = CHUNK_NEXT(next_chunk);
		}
		return NULL;
	}

	void MemCache::Free( void * ptr )
	{
		size_t this_chunk = (size_t)ptr - ALIGN_BYTES;
		size_t prev_chunk = 0;
		size_t next_chunk = _first_free;
		while(next_chunk > 0 && next_chunk < this_chunk)
		{
			prev_chunk = next_chunk;
			next_chunk = CHUNK_NEXT(next_chunk);
		}
		size_t this_size = CHUNK_USED(this_chunk);
		if(prev_chunk == 0)
		{
			_first_free = this_chunk;
			if(this_chunk + this_size == next_chunk)
			{
				CHUNK_NEXT(this_chunk) = CHUNK_NEXT(next_chunk);
				CHUNK_SIZE(this_chunk) = this_size + CHUNK_SIZE(next_chunk);
			}
			else
			{
				CHUNK_NEXT(this_chunk) = next_chunk;
				CHUNK_SIZE(this_chunk) = this_size;
			}
		}
		else
		{
			size_t prev_size = CHUNK_SIZE(prev_chunk);
			if(prev_chunk + prev_size == this_chunk)
			{
				if(this_chunk + this_size == next_chunk)
				{
					CHUNK_NEXT(prev_chunk) = CHUNK_NEXT(next_chunk);
					CHUNK_SIZE(prev_chunk) = prev_size + this_size + CHUNK_SIZE(next_chunk);
				}
				else
				{
					CHUNK_NEXT(prev_chunk) = next_chunk;
					CHUNK_SIZE(prev_chunk) = prev_size + this_size;
				}
			}
			else
			{
				CHUNK_NEXT(prev_chunk) = this_chunk;
				if(this_chunk + this_size == next_chunk)
				{
					CHUNK_NEXT(this_chunk) = CHUNK_NEXT(next_chunk);
					CHUNK_SIZE(this_chunk) = this_size + CHUNK_SIZE(next_chunk);
				}
				else
				{
					CHUNK_NEXT(this_chunk) = next_chunk;
					CHUNK_SIZE(this_chunk) = this_size;
				}
			}
		}
	}

	void MemCache::PrintDebugInfo()
	{
		printf("Free chunks:\n");
		size_t chunk = _first_free;
		while(chunk != 0)
		{
			printf("\tOffset: %08X\tSize: %08X\n", chunk, CHUNK_SIZE(chunk));
			chunk = CHUNK_NEXT(chunk);
		}
	}
}
