#include "RWLock_POSIX.h"


namespace Utils {


RWLockImpl::RWLockImpl()
{
	if (pthread_rwlock_init(&_rwl, NULL))
		throw ("cannot create reader/writer lock");
}


RWLockImpl::~RWLockImpl()
{
	pthread_rwlock_destroy(&_rwl);
}


} // namespace Utils
