#ifndef _RWLock_INCLUDED
#define _RWLock_INCLUDED


#if defined(_WIN32)
#include "RWLock_WIN32.h"
#else
#include "RWLock_POSIX.h"
#endif


namespace Utils {


class ScopedRWLock;


class RWLock: private RWLockImpl
	/// A reader writer lock allows multiple concurrent
	/// readers or one exclusive writer.
{
public:
	typedef ScopedRWLock ScopedLock;

	RWLock();
		/// Creates the Reader/Writer lock.
		
	~RWLock();
		/// Destroys the Reader/Writer lock.
	
	void readLock();
		/// Acquires a read lock. If another thread currently holds a write lock,
		/// waits until the write lock is released.

	bool tryReadLock();
		/// Tries to acquire a read lock. Immediately returns true if successful, or
		/// false if another thread currently holds a write lock.

	void writeLock();
		/// Acquires a write lock. If one or more other threads currently hold 
		/// locks, waits until all locks are released. The results are undefined
		/// if the same thread already holds a read or write lock

	bool tryWriteLock();
		/// Tries to acquire a write lock. Immediately returns true if successful,
		/// or false if one or more other threads currently hold 
		/// locks. The result is undefined if the same thread already
		/// holds a read or write lock.

	void unlock();
		/// Releases the read or write lock.

private:
	RWLock(const RWLock&);
	RWLock& operator = (const RWLock&);
};


class ScopedRWLock
	/// A variant of ScopedLock for reader/writer locks.
{
public:
	ScopedRWLock(RWLock& rwl, bool write = false);
	~ScopedRWLock();

private:
	RWLock& _rwl;

	ScopedRWLock();
	ScopedRWLock(const ScopedRWLock&);
	ScopedRWLock& operator = (const ScopedRWLock&);
};


//
// inlines
//
inline void RWLock::readLock()
{
	readLockImpl();
}


inline bool RWLock::tryReadLock()
{
	return tryReadLockImpl();
}


inline void RWLock::writeLock()
{
	writeLockImpl();
}


inline bool RWLock::tryWriteLock()
{
	return tryWriteLockImpl();
}


inline void RWLock::unlock()
{
	unlockImpl();
}


inline ScopedRWLock::ScopedRWLock(RWLock& rwl, bool write): _rwl(rwl)
{
	if (write)
		_rwl.writeLock();
	else
		_rwl.readLock();
}


inline ScopedRWLock::~ScopedRWLock()
{
	_rwl.unlock();
}


} // namespace Utils


#endif // _RWLock_INCLUDED
