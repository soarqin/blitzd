#ifndef _RWLock_POSIX_INCLUDED
#define _RWLock_POSIX_INCLUDED


#include <pthread.h>
#include <errno.h>


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
	pthread_rwlock_t _rwl;
};


//
// inlines
//
inline void RWLockImpl::readLockImpl()
{
	if (pthread_rwlock_rdlock(&_rwl)) 
		throw ("cannot lock reader/writer lock");
}


inline bool RWLockImpl::tryReadLockImpl()
{
	int rc = pthread_rwlock_tryrdlock(&_rwl);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
		throw ("cannot lock reader/writer lock");

}


inline void RWLockImpl::writeLockImpl()
{
	if (pthread_rwlock_wrlock(&_rwl)) 
		throw ("cannot lock reader/writer lock");
}


inline bool RWLockImpl::tryWriteLockImpl()
{
	int rc = pthread_rwlock_trywrlock(&_rwl);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
		throw ("cannot lock reader/writer lock");

}


inline void RWLockImpl::unlockImpl()
{
	if (pthread_rwlock_unlock(&_rwl))
		throw ("cannot unlock mutex");
}


} // namespace Utils


#endif // _RWLock_POSIX_INCLUDED
