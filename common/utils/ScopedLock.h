#ifndef _ScopedLock_INCLUDED
#define _ScopedLock_INCLUDED

namespace Utils {


template <class M>
class ScopedLock
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a Mutex and locks it.
	/// The destructor unlocks the mutex.
{
public:
	inline ScopedLock(M& mutex): _mutex(mutex)
	{
		_mutex.lock();
	}
	inline ~ScopedLock()
	{
		_mutex.unlock();
	}

private:
	M& _mutex;

	ScopedLock();
	ScopedLock(const ScopedLock&);
	ScopedLock& operator = (const ScopedLock&);
};


} // namespace Utils


#endif // _ScopedLock_INCLUDED
