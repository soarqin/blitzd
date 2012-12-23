#ifndef Thread_INCLUDED
#define Thread_INCLUDED

#include "OSPlatform.h"
#include "Mutex.h"


#if defined(UTILS_OS_FAMILY_WINDOWS)
#include "Thread_WIN32.h"
#else
#include "Thread_POSIX.h"
#endif


namespace Utils {


class Runnable;
class ThreadLocalStorage;


class Thread: private ThreadImpl
	/// This class implements a platform-independent
	/// wrapper to an operating system thread.
	///
	/// Every Thread object gets a unique (within
	/// its process) numeric thread ID.
	/// Furthermore, a thread can be assigned a name.
	/// The name of a thread can be changed at any time.
{
public:	
	using ThreadImpl::Callable;

	enum Priority
		/// Thread priorities.
	{
		PRIO_LOWEST  = PRIO_LOWEST_IMPL, /// The lowest thread priority.
		PRIO_LOW     = PRIO_LOW_IMPL,    /// A lower than normal thread priority.
		PRIO_NORMAL  = PRIO_NORMAL_IMPL, /// The normal thread priority.
		PRIO_HIGH    = PRIO_HIGH_IMPL,   /// A higher than normal thread priority.
		PRIO_HIGHEST = PRIO_HIGHEST_IMPL /// The highest thread priority.
	};

	Thread();
		/// Creates a thread. Call start() to start it.
		
	Thread(const std::string& name);
		/// Creates a named thread. Call start() to start it.
		
	~Thread();
		/// Destroys the thread.

	int id() const;
		/// Returns the unique thread ID of the thread.

	std::string name() const;
		/// Returns the name of the thread.

	std::string getName() const;
		/// Returns teh name of the thread.

	void setName(const std::string& name);
		/// Sets the name of the thread.

	void setPriority(Priority prio);
		/// Sets the thread's priority.
		///
		/// Some platform only allow changing a thread's priority
		/// if the process has certain privileges.

	Priority getPriority() const;
		/// Returns the thread's priority.

	void setOSPriority(int prio);
		/// Sets the thread's priority, using an operating system specific
		/// priority value. Use getMinOSPriority() and getMaxOSPriority() to
		/// obtain mininum and maximum priority values.
		
	int getOSPriority() const;
		/// Returns the thread's priority, expressed as an operating system
		/// specific priority value.
		
	static int getMinOSPriority();
		/// Returns the mininum operating system-specific priority value,
		/// which can be passed to setOSPriority().
		
	static int getMaxOSPriority();
		/// Returns the maximum operating system-specific priority value,
		/// which can be passed to setOSPriority().

	void setStackSize(int size);
		/// Sets the thread's stack size in bytes.
		/// Setting the stack size to 0 will use the default stack size.
		/// Typically, the real stack size is rounded up to the nearest
		/// page size multiple.

	int getStackSize() const;
		/// Returns the thread's stack size in bytes.
		/// If the default stack size is used, 0 is returned.

	void start(Runnable& target);
		/// Starts the thread with the given target.

	void start(Callable target, void* pData = 0);
		/// Starts the thread with the given target and parameter.

	void join();
		/// Waits until the thread completes execution.	
		/// If multiple threads try to join the same
		/// thread, the result is undefined.
		
	void join(long milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Throws a TimeoutException if the thread
		/// does not complete within the specified time interval.
		
	bool tryJoin(long milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Returns true if the thread has finished,
		/// false otherwise.

	bool isRunning() const;
		/// Returns true if the thread is running.

	static void sleep(long milliseconds);
		/// Suspends the current thread for the specified
		/// amount of time.

	static void yield();
		/// Yields cpu to other threads.

	static Thread* current();
		/// Returns the Thread object for the currently active thread.
		/// If the current thread is the main thread, 0 is returned.

protected:
	ThreadLocalStorage& tls();
		/// Returns a reference to the thread's local storage.

	void clearTLS();
		/// Clears the thread's local storage.

	std::string makeName();
		/// Creates a unique name for a thread.
		
	static int uniqueId();
		/// Creates and returns a unique id for a thread.
		
private:
	Thread(const Thread&);
	Thread& operator = (const Thread&);

	int                 _id;
	std::string         _name;
	ThreadLocalStorage* _pTLS;
	mutable FastMutex   _mutex;

	friend class ThreadLocalStorage;
	friend class PooledThread;
};


//
// inlines
//
inline int Thread::id() const
{
	return _id;
}


inline std::string Thread::name() const
{
	FastMutex::ScopedLock lock(_mutex);
	
	return _name;
}


inline std::string Thread::getName() const
{
	FastMutex::ScopedLock lock(_mutex);
	
	return _name;
}


inline bool Thread::isRunning() const
{
	return isRunningImpl();
}


inline void Thread::sleep(long milliseconds)
{
	sleepImpl(milliseconds);
}


inline void Thread::yield()
{
	yieldImpl();
}


inline Thread* Thread::current()
{
	return static_cast<Thread*>(currentImpl());
}


inline void Thread::setOSPriority(int prio)
{
	setOSPriorityImpl(prio);	
}

	
inline int Thread::getOSPriority() const
{
	return getOSPriorityImpl();
}

	
inline int Thread::getMinOSPriority()
{
	return ThreadImpl::getMinOSPriorityImpl();
}

	
inline int Thread::getMaxOSPriority()
{
	return ThreadImpl::getMaxOSPriorityImpl();
}


inline void Thread::setStackSize(int size)
{
	setStackSizeImpl(size);
}


inline int Thread::getStackSize() const
{
	return getStackSizeImpl();
}


} // namespace Utils


#endif // Thread_INCLUDED
