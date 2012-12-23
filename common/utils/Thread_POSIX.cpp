#include "Thread_POSIX.h"
#include "Timespan.h"
#include "Timestamp.h"
#include <signal.h>
#if defined(__sun) && defined(__SVR4)
#	if !defined(__EXTENSIONS__)
#		define __EXTENSIONS__
#	endif
#endif


//
// Block SIGPIPE in main thread.
//
#if defined(UTILS_OS_FAMILY_UNIX)
namespace
{
	class SignalBlocker
	{
	public:
		SignalBlocker()
		{
			sigset_t sset;
			sigemptyset(&sset);
			sigaddset(&sset, SIGPIPE); 
			pthread_sigmask(SIG_BLOCK, &sset, 0);
		}
		~SignalBlocker()
		{
		}
	};
	
	static SignalBlocker signalBlocker;
}
#endif


namespace Utils {


ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;


ThreadImpl::ThreadImpl():
	_pData(new ThreadData)
{
}

			
ThreadImpl::~ThreadImpl()
{
	if (isRunningImpl())
		pthread_detach(_pData->thread);
}


void ThreadImpl::setPriorityImpl(int prio)
{
	if (prio != _pData->prio)
	{
		_pData->prio = prio;
		if (isRunningImpl())
		{
			struct sched_param par;
			par.sched_priority = mapPrio(_pData->prio);
			if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
				throw ("cannot set thread priority");
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio)
{
	if (prio != _pData->osPrio)
	{
		if (_pData->pRunnableTarget || _pData->pCallbackTarget)
		{
			struct sched_param par;
			par.sched_priority = prio;
			if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
				throw ("cannot set thread priority");
		}
		_pData->prio   = reverseMapPrio(prio);
		_pData->osPrio = prio;
	}
}


int ThreadImpl::getMinOSPriorityImpl()
{
#if defined(__VMS) || defined(__digital__)
	return PRI_OTHER_MIN;
#else
	return sched_get_priority_min(SCHED_OTHER);
#endif
}


int ThreadImpl::getMaxOSPriorityImpl()
{
#if defined(__VMS) || defined(__digital__)
	return PRI_OTHER_MAX;
#else
	return sched_get_priority_max(SCHED_OTHER);
#endif
}


void ThreadImpl::setStackSizeImpl(int size)
{
#ifndef PTHREAD_STACK_MIN
	_pData->stackSize = 0;
#else
 	if (size != 0)
 	{
#if defined(__APPLE__)
		// we must round up to a multiple of the memory page size
		const int PAGE_SIZE = 4096;
		size = ((size + PAGE_SIZE - 1)/PAGE_SIZE)*PAGE_SIZE;
#endif
 		if (size < PTHREAD_STACK_MIN)
 			size = PTHREAD_STACK_MIN;
	}
 	_pData->stackSize = size;
#endif
}


void ThreadImpl::startImpl(Runnable& target)
{
	if (_pData->pRunnableTarget)
		throw ("thread already running");

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (_pData->stackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, _pData->stackSize))
			throw ("cannot set thread stack size");
	}

	_pData->pRunnableTarget = &target;
	if (pthread_create(&_pData->thread, &attributes, runnableEntry, this))
	{
		_pData->pRunnableTarget = 0;
		throw ("cannot start thread");
	}

	if (_pData->prio != PRIO_NORMAL_IMPL)
	{
		struct sched_param par;
		par.sched_priority = mapPrio(_pData->prio);
		if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
			throw ("cannot set thread priority");
	}
}


void ThreadImpl::startImpl(Callable target, void* pData)
{
	if (_pData->pCallbackTarget && _pData->pCallbackTarget->callback)
		throw ("thread already running");

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (_pData->stackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, _pData->stackSize))
			throw ("can not set thread stack size");
	}

	if (0 == _pData->pCallbackTarget.get())
		_pData->pCallbackTarget = new CallbackData;

	_pData->pCallbackTarget->callback = target;
	_pData->pCallbackTarget->pData = pData;

	if (pthread_create(&_pData->thread, &attributes, callableEntry, this))
	{
		_pData->pCallbackTarget->callback = 0;
		_pData->pCallbackTarget->pData = 0;
		throw ("cannot start thread");
	}

	if (_pData->prio != PRIO_NORMAL_IMPL)
	{
		struct sched_param par;
		par.sched_priority = mapPrio(_pData->prio);
		if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
			throw ("cannot set thread priority");
	}
}


void ThreadImpl::joinImpl()
{
	_pData->done.wait();
	void* result;
	if (pthread_join(_pData->thread, &result))
		throw ("cannot join thread"); 
}


bool ThreadImpl::joinImpl(long milliseconds)
{
	if (_pData->done.tryWait(milliseconds))
	{
		void* result;
		if (pthread_join(_pData->thread, &result))
			throw ("cannot join thread");
		return true;
	}
	else return false;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	return _currentThreadHolder.get();
}


void ThreadImpl::sleepImpl(long milliseconds)
{
#if defined(__VMS) || defined(__digital__)
		// This is specific to DECThreads
		struct timespec interval;
		interval.tv_sec  = milliseconds / 1000;
		interval.tv_nsec = (milliseconds % 1000)*1000000; 
		pthread_delay_np(&interval);
#else 
	Utils::Timespan remainingTime(1000*Utils::Timespan::TimeDiff(milliseconds));
	int rc;
	do
	{
		struct timeval tv;
		tv.tv_sec  = (long) remainingTime.totalSeconds();
		tv.tv_usec = (long) remainingTime.useconds();
		Utils::Timestamp start;
		rc = ::select(0, NULL, NULL, NULL, &tv);
		if (rc < 0 && errno == EINTR)
		{
			Utils::Timestamp end;
			Utils::Timespan waited = start.elapsed();
			if (waited < remainingTime)
				remainingTime -= waited;
			else
				remainingTime = 0;
		}
	}
	while (remainingTime > 0 && rc < 0 && errno == EINTR);
	if (rc < 0) throw ("Thread::sleep(): select() failed");
#endif
}


void* ThreadImpl::runnableEntry(void* pThread)
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));

#if defined(UTILS_OS_FAMILY_UNIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE); 
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

	ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
	AutoPtr<ThreadData> pData = pThreadImpl->_pData;
	try
	{
		pData->pRunnableTarget->run();
	}
	catch (...)
	{
	}

	pData->pRunnableTarget = 0;
	pData->done.set();
	return 0;
}


void* ThreadImpl::callableEntry(void* pThread)
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));

#if defined(UTILS_OS_FAMILY_UNIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE); 
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

	ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
	AutoPtr<ThreadData> pData = pThreadImpl->_pData;
	try
	{
		pData->pCallbackTarget->callback(pData->pCallbackTarget->pData);
	}
	catch (...)
	{
	}

	pData->pCallbackTarget->callback = 0;
	pData->pCallbackTarget->pData = 0;

	pData->done.set();
	return 0;
}


int ThreadImpl::mapPrio(int prio)
{
	int pmin = getMinOSPriorityImpl();
	int pmax = getMaxOSPriorityImpl();

	switch (prio)
	{
	case PRIO_LOWEST_IMPL:
		return pmin;
	case PRIO_LOW_IMPL:
		return pmin + (pmax - pmin)/4;
	case PRIO_NORMAL_IMPL:
		return pmin + (pmax - pmin)/2;
	case PRIO_HIGH_IMPL:
		return pmin + 3*(pmax - pmin)/4;
	case PRIO_HIGHEST_IMPL:
		return pmax;
	default:;
		//UTILS_bugcheck_msg("invalid thread priority");
	}
	return -1; // just to satisfy compiler - we'll never get here anyway
}


int ThreadImpl::reverseMapPrio(int prio)
{
	int pmin = getMinOSPriorityImpl();
	int pmax = getMaxOSPriorityImpl();
	int normal = pmin + (pmax - pmin)/2;
	if (prio == pmax)
		return PRIO_HIGHEST_IMPL;
	if (prio > normal)
		return PRIO_HIGH_IMPL;
	else if (prio == normal)
		return PRIO_NORMAL_IMPL;
	else if (prio > pmin)
		return PRIO_LOW_IMPL;
	else
		return PRIO_LOWEST_IMPL;
}


} // namespace Utils
