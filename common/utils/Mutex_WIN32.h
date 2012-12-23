#ifndef _Mutex_WIN32_INCLUDED
#define _Mutex_WIN32_INCLUDED


namespace Utils {


class MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	CRITICAL_SECTION _cs;
};


typedef MutexImpl FastMutexImpl;


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&_cs);
	}
	catch (...)
	{
		throw ("cannot lock mutex");
	}
}


inline bool MutexImpl::tryLockImpl()
{
	try
	{
		return TryEnterCriticalSection(&_cs) == TRUE;
	}
	catch (...)
	{
	}
	throw ("cannot lock mutex");
}


inline void MutexImpl::unlockImpl()
{
	LeaveCriticalSection(&_cs);
}


} // namespace Utils


#endif // _Mutex_WIN32_INCLUDED
