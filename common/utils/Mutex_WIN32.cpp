#include "Mutex_WIN32.h"
#include "Timestamp.h"


namespace Utils {


MutexImpl::MutexImpl()
{
	// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
	// the return only checks if the input address of &_cs was valid, so it is safe to omit it
	InitializeCriticalSectionAndSpinCount(&_cs, 4000);
}


MutexImpl::~MutexImpl()
{
	DeleteCriticalSection(&_cs);
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
	const int sleepMillis = 5;
	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
	do
	{
		try
		{
			if (TryEnterCriticalSection(&_cs) == TRUE)
				return true;
		}
		catch (...)
		{
			throw ("cannot lock mutex");
		}
		Sleep(sleepMillis);
	}
	while (!now.isElapsed(diff));
	return false;
}


} // namespace Utils
