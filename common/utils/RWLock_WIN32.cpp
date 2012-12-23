#include "RWLock_WIN32.h"


namespace Utils {


RWLockImpl::RWLockImpl(): _readers(0), _writersWaiting(0), _writers(0)
{
	_mutex = CreateMutexW(NULL, FALSE, NULL);
	if (_mutex == NULL)
		throw ("cannot create reader/writer lock");

	_readEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (_readEvent == NULL)
		throw ("cannot create reader/writer lock");

	_writeEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (_writeEvent == NULL)
		throw ("cannot create reader/writer lock");
}


RWLockImpl::~RWLockImpl()
{
	CloseHandle(_mutex);
	CloseHandle(_readEvent);
	CloseHandle(_writeEvent);
}


inline void RWLockImpl::addWriter()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (++_writersWaiting == 1) ResetEvent(_readEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw ("cannot lock reader/writer lock");
	}
}


inline void RWLockImpl::removeWriter()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (--_writersWaiting == 0 && _writers == 0) SetEvent(_readEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw ("cannot lock reader/writer lock");
	}
}


void RWLockImpl::readLockImpl()
{
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _readEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++_readers;
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw ("cannot lock reader/writer lock");
	}
}


bool RWLockImpl::tryReadLockImpl()
{
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _readEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++_readers;
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
		return true;
	case WAIT_TIMEOUT:
		return false;
	default:
		throw ("cannot lock reader/writer lock");
	}
}


void RWLockImpl::writeLockImpl()
{
	addWriter();
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--_writersWaiting;
		++_readers;
		++_writers;
		ResetEvent(_readEvent);
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		removeWriter();
		throw ("cannot lock reader/writer lock");
	}
}


bool RWLockImpl::tryWriteLockImpl()
{
	addWriter();
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--_writersWaiting;
		++_readers;
		++_writers;
		ResetEvent(_readEvent);
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
		return true;
	case WAIT_TIMEOUT:
		removeWriter();
		return false;
	default:
		removeWriter();
		throw ("cannot lock reader/writer lock");
	}
}


void RWLockImpl::unlockImpl()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		_writers = 0;
		if (_writersWaiting == 0) SetEvent(_readEvent);
		if (--_readers == 0) SetEvent(_writeEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw ("cannot unlock reader/writer lock");
	}
}


} // namespace Utils
