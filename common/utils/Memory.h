#ifndef _MEMORY_H_
#define _MEMORY_H_

namespace Utils
{
	typedef struct {
		void * (*Alloc)(int size);
		void * (*Calloc)(int size);
		void * (*ReAlloc)(void * buf, int size);
		void (*Free)(void * buf);
	} MEM_t;

	typedef struct {
		int (*strlen)(const char * str);
		void (*strcpy)(char * dst, const char * src);
		void (*strcpyn)(char * dst, const char * src, int size);
		int (*strcmp)(const char * s1, const char * s2);
		int (*strcmpi)(const char * s1, const char * s2);
		int (*strcmpn)(const char * s1, const char * s2, int size);
		int (*strcmpni)(const char * s1, const char * s2, int size);
		char * (*strdup)(const char * s);
	} STR_t;
}

extern Utils::MEM_t _MEM;
extern Utils::STR_t _STR;

#endif // _MEMORY_H_
