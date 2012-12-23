#ifndef Thread_WIN32_INCLUDED
#define Thread_WIN32_INCLUDED


#include "Runnable.h"


namespace Utils {


class ThreadImpl
{
public:	
	typedef void (*Callable)(void*);

#if defined(_DLL)
	typedef DWORD (WINAPI *Entry)(LPVOID);
#else
	typedef unsigned (__stdcall *Entry)(void*);
#endif

	struct CallbackData
	{
		CallbackData(): callback(0), pData(0)
		{
		}

		Callable  callback;
		void*     pData; 
	};

	enum Priority
	{
		PRIO_LOWEST_IMPL  = THREAD_PRIORITY_LOWEST,
		PRIO_LOW_IMPL     = THREAD_PRIORITY_BELOW_NORMAL,
		PRIO_NORMAL_IMPL  = THREAD_PRIORITY_NORMAL,
		PRIO_HIGH_IMPL    = THREAD_PRIORITY_ABOVE_NORMAL,
		PRIO_HIGHEST_IMPL = THREAD_PRIORITY_HIGHEST
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
#if defined(_DLL)
	static DWORD WINAPI runnableEntry(LPVOID pThread);
#else
	static unsigned __stdcall runnableEntry(void* pThread);
#endif

#if defined(_DLL)
	static DWORD WINAPI callableEntry(LPVOID pThread);
#else
	static unsigned __stdcall callableEntry(void* pThread);
#endif

	void createImpl(Entry ent, void* pData);
	void threadCleanup();

private:
	class CurrentThreadHolder
	{
	public:
		CurrentThreadHolder(): _slot(TlsAlloc())
		{
			if (_slot == TLS_OUT_OF_INDEXES)
				throw ("cannot allocate thread context key");
		}
		~CurrentThreadHolder()
		{
			TlsFree(_slot);
		}
		ThreadImpl* get() const
		{
			return reinterpret_cast<ThreadImpl*>(TlsGetValue(_slot));
		}
		void set(ThreadImpl* pThread)
		{
			TlsSetValue(_slot, pThread);
		}
	
	private:
		DWORD _slot;
	};

	Runnable*    _pRunnableTarget;
	CallbackData _callbackTarget;
	HANDLE       _thread;
	int          _prio;
	int          _stackSize;

	static CurrentThreadHolder _currentThreadHolder;
};


//
// inlines
//
inline int ThreadImpl::getPriorityImpl() const
{
	return _prio;
}


inline int ThreadImpl::getOSPriorityImpl() const
{
	return _prio;
}


inline int ThreadImpl::getMinOSPriorityImpl()
{
	return PRIO_LOWEST_IMPL;
}


inline int ThreadImpl::getMaxOSPriorityImpl()
{
	return PRIO_HIGHEST_IMPL;
}


inline void ThreadImpl::sleepImpl(long milliseconds)
{
	Sleep(DWORD(milliseconds));
}


inline void ThreadImpl::yieldImpl()
{
	Sleep(0);
}


inline void ThreadImpl::setStackSizeImpl(int size)
{
	_stackSize = size;
}


inline int ThreadImpl::getStackSizeImpl() const
{
	return _stackSize;
}


} // namespace Utils


#endif // Thread_WIN32_INCLUDED
