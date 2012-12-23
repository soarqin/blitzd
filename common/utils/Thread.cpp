#include "Config.h"
#include "PreConfig.h"
#include "Thread.h"
#include "Mutex.h"
#include "ThreadLocal.h"
#include "Exception.h"
#include <sstream>


#if defined(UTILS_OS_FAMILY_WINDOWS)
#include "Thread_WIN32.cpp"
#else
#include "Thread_POSIX.cpp"
#endif


namespace Utils {


Thread::Thread(): 
	_id(uniqueId()), 
	_name(makeName()), 
	_pTLS(0)
{
}


Thread::Thread(const std::string& name): 
	_id(uniqueId()), 
	_name(name), 
	_pTLS(0)
{
}


Thread::~Thread()
{
	delete _pTLS;
}


void Thread::setPriority(Priority prio)
{
	setPriorityImpl(prio);
}


Thread::Priority Thread::getPriority() const
{
	return Priority(getPriorityImpl());
}


void Thread::start(Runnable& target)
{
	startImpl(target);
}


void Thread::start(Callable target, void* pData)
{
	startImpl(target, pData);
}


void Thread::join()
{
	joinImpl();
}


void Thread::join(long milliseconds)
{
	if (!joinImpl(milliseconds))
		throw StringException("Timeout");
}


bool Thread::tryJoin(long milliseconds)
{
	return joinImpl(milliseconds);
}


ThreadLocalStorage& Thread::tls()
{
	if (!_pTLS)
		_pTLS = new ThreadLocalStorage;
	return *_pTLS;
}


void Thread::clearTLS()
{
	if (_pTLS)
	{
		delete _pTLS;
		_pTLS = 0;
	}
}


std::string Thread::makeName()
{
	std::ostringstream name;
	name << '#' << _id;
	return name.str();
}


int Thread::uniqueId()
{
	static FastMutex mtx;
	FastMutex::ScopedLock lock(mtx);

	static unsigned count = 0;
	++count;
	return count;
}


void Thread::setName(const std::string& name)
{
	FastMutex::ScopedLock lock(_mutex);

	_name = name;
}


} // namespace Utils
