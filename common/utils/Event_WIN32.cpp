#include "Event_WIN32.h"


namespace Utils {


EventImpl::EventImpl(bool autoReset)
{
	_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
	if (!_event)
		throw ("cannot create event");
}


EventImpl::~EventImpl()
{
	CloseHandle(_event);
}


void EventImpl::waitImpl()
{
	switch (WaitForSingleObject(_event, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		throw ("wait for event failed");
	}
}


bool EventImpl::waitImpl(long milliseconds)
{
	switch (WaitForSingleObject(_event, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw ("wait for event failed");		
	}
}


} // namespace Utils
