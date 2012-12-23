#include "Config.h"
#include "PreConfig.h"
#include "Event.h"


#if defined(UTILS_OS_FAMILY_WINDOWS)
#include "Event_WIN32.cpp"
#else
#include "Event_POSIX.cpp"
#endif


namespace Utils {


Event::Event(bool autoReset): EventImpl(autoReset)
{
}


Event::~Event()
{
}


} // namespace Utils
