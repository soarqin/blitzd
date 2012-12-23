#include "PreConfig.h"
#include "Thread_WIN32.h"
#include <process.h>


namespace Utils {


ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;


ThreadImpl::ThreadImpl():
	_pRunnableTarget(0),
	_thread(0),
	_prio(PRIO_NORMAL_IMPL),
	_stackSize(UTILS_THREAD_STACK_SIZE)
{
}

			
ThreadImpl::~ThreadImpl()
{
	if (_thread) CloseHandle(_thread);
}


void ThreadImpl::setPriorityImpl(int prio)
{
	if (prio != _prio)
	{
		_prio = prio;
		if (_thread)
		{
			if (SetThreadPriority(_thread, _prio) == 0)
				throw ("cannot set thread priority");
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio)
{
	setPriorityImpl(prio);
}


void ThreadImpl::startImpl(Runnable& target)
{
	if (isRunningImpl())
		throw ("thread already running");

	_pRunnableTarget = &target;

	createImpl(runnableEntry, this);
}


void ThreadImpl::startImpl(Callable target, void* pData)
{
	if (isRunningImpl())
		throw ("thread already running");

	_callbackTarget.callback = target;
	_callbackTarget.pData = pData;

	createImpl(callableEntry, this);
}


void ThreadImpl::createImpl(Entry ent, void* pData)
{
#if defined(_DLL)
	DWORD threadId;
	_thread = CreateThread(NULL, _stackSize, ent, pData, 0, &threadId);
#else
	unsigned threadId;
	_thread = (HANDLE) _beginthreadex(NULL, _stackSize, ent, this, 0, &threadId);
#endif
	if (!_thread)
		throw ("cannot create thread");
	if (_prio != PRIO_NORMAL_IMPL && !SetThreadPriority(_thread, _prio))
		throw ("cannot set thread priority");
}


void ThreadImpl::joinImpl()
{
	if (!_thread) return;

	switch (WaitForSingleObject(_thread, INFINITE))
	{
	case WAIT_OBJECT_0:
		threadCleanup();
		return;
	default:
		throw ("cannot join thread");
	}
}


bool ThreadImpl::joinImpl(long milliseconds)
{
	if (!_thread) return true;

	switch (WaitForSingleObject(_thread, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		threadCleanup();
		return true;
	default:
		throw ("cannot join thread");
	}
}


bool ThreadImpl::isRunningImpl() const
{
	if (_thread)
	{
		DWORD ec = 0;
		return GetExitCodeThread(_thread, &ec) && ec == STILL_ACTIVE;
	}
	return false;
}


void ThreadImpl::threadCleanup()
{
	if (!_thread) return;
	if (CloseHandle(_thread)) _thread = 0;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	return _currentThreadHolder.get();
}


#if defined(_DLL)
DWORD WINAPI ThreadImpl::runnableEntry(LPVOID pThread)
#else
unsigned __stdcall ThreadImpl::runnableEntry(void* pThread)
#endif
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));
	try
	{
		reinterpret_cast<ThreadImpl*>(pThread)->_pRunnableTarget->run();
	}
	catch (...)
	{
	}
	return 0;
}


#if defined(_DLL)
DWORD WINAPI ThreadImpl::callableEntry(LPVOID pThread)
#else
unsigned __stdcall ThreadImpl::callableEntry(void* pThread)
#endif
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));
	try
	{
		ThreadImpl* pTI = reinterpret_cast<ThreadImpl*>(pThread);
		pTI->_callbackTarget.callback(pTI->_callbackTarget.pData);
	}
	catch (...)
	{
	}
	return 0;
}


} // namespace Utils
