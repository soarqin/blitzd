#ifndef Thread_POSIX_INCLUDED
#define Thread_POSIX_INCLUDED


#include "Config.h"
#include "PreConfig.h"
#include "Runnable.h"
#include "SignalHandler.h"
#include "Event.h"
#include "RefCountedObject.h"
#include "AutoPtr.h"
#include <pthread.h>
// must be limits.h (not <climits>) for PTHREAD_STACK_MIN on Solaris
#include <limits.h>
#include <sys/select.h>
#include <errno.h>


namespace Utils {


class ThreadImpl
{
public:	
	typedef void (*Callable)(void*);

	enum Priority
	{
		PRIO_LOWEST_IMPL,
		PRIO_LOW_IMPL,
		PRIO_NORMAL_IMPL,
		PRIO_HIGH_IMPL,
		PRIO_HIGHEST_IMPL
	};

	struct CallbackData: public RefCountedObject
	{
		CallbackData(): callback(0), pData(0)
		{
		}

		Callable  callback;
		void*     pData; 
	};

	ThreadImpl();				
	~ThreadImpl();

	void setPriorityImpl(int prio);
	int getPriorityImpl() const;
	void setOSPriorityImpl(int prio);
	int getOSPriorityImpl() const;
	static int getMinOSPriorityImpl();
	static int getMaxOSPriorityImpl();
	void setStackSizeImpl(int size);
	int getStackSizeImpl() const;
	void startImpl(Runnable& target);
	void startImpl(Callable target, void* pData = 0);

	void joinImpl();
	bool joinImpl(long milliseconds);
	bool isRunningImpl() const;
	static void sleepImpl(long milliseconds);
	static void yieldImpl();
	static ThreadImpl* currentImpl();

protected:
	static void* runnableEntry(void* pThread);
	static void* callableEntry(void* pThread);
	static int mapPrio(int prio);
	static int reverseMapPrio(int osPrio);

private:
	class CurrentThreadHolder
	{
	public:
		CurrentThreadHolder()
		{
			if (pthread_key_create(&_key, NULL))
				throw ("cannot allocate thread context key");
		}
		~CurrentThreadHolder()
		{
			pthread_key_delete(_key);
		}
		ThreadImpl* get() const
		{
			return reinterpret_cast<ThreadImpl*>(pthread_getspecific(_key));
		}
		void set(ThreadImpl* pThread)
		{
			pthread_setspecific(_key, pThread);
		}
	
	private:
		pthread_key_t _key;
	};

	struct ThreadData: public RefCountedObject
	{
		ThreadData():
			pRunnableTarget(0),
			pCallbackTarget(0),
			thread(0),
			prio(PRIO_NORMAL_IMPL),
			done(false),
			stackSize(UTILS_THREAD_STACK_SIZE)
		{
		}

		Runnable*     pRunnableTarget;
		AutoPtr<CallbackData> pCallbackTarget;
		pthread_t     thread;
		int           prio;
		int           osPrio;
		Event         done;
		std::size_t   stackSize;
	};

	AutoPtr<ThreadData> _pData;

	static CurrentThreadHolder _currentThreadHolder;
	
#if defined(UTILS_OS_FAMILY_UNIX)
	SignalHandler::JumpBufferVec _jumpBufferVec;
	friend class SignalHandler;
#endif
};


//
// inlines
//
inline int ThreadImpl::getPriorityImpl() const
{
	return _pData->prio;
}


inline int ThreadImpl::getOSPriorityImpl() const
{
	return _pData->osPrio;
}


inline bool ThreadImpl::isRunningImpl() const
{
	return _pData->pRunnableTarget != 0 ||
		(_pData->pCallbackTarget.get() != 0 && _pData->pCallbackTarget->callback != 0);
}


inline void ThreadImpl::yieldImpl()
{
	sched_yield();
}


inline int ThreadImpl::getStackSizeImpl() const
{
	return _pData->stackSize;
}


} // namespace Utils


#endif // Thread_POSIX_INCLUDED
