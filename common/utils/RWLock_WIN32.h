#ifndef _RWLock_WIN32_INCLUDED
#define _RWLock_WIN32_INCLUDED

#include <windows.h>

namespace Utils {


class RWLockImpl
{
protected:
	RWLockImpl();
	~RWLockImpl();
	void readLockImpl();
	bool tryReadLockImpl();
	void writeLockImpl();
	bool tryWriteLockImpl();
	void unlockImpl();
	
private:
	void addWriter();
	void removeWriter();

	HANDLE   _mutex;
	HANDLE   _readEvent;
	HANDLE   _writeEvent;
	unsigned _readers;
	unsigned _writersWaiting;
	unsigned _writers;
};


} // namespace Utils


#endif // _RWLock_WIN32_INCLUDED
