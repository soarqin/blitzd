#include "Config.h"
#include "Timestamp.h"
#include "Exception.h"
#include <algorithm>
#if !defined(_WIN32)
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif


namespace Utils {


Timestamp::Timestamp()
{
	update();
}


Timestamp::Timestamp(TimeVal tv)
{
	_ts = tv;
}


Timestamp::Timestamp(const Timestamp& other)
{
	_ts = other._ts;
}


Timestamp::~Timestamp()
{
}


Timestamp& Timestamp::operator = (const Timestamp& other)
{
	_ts = other._ts;
	return *this;
}


Timestamp& Timestamp::operator = (TimeVal tv)
{
	_ts = tv;
	return *this;
}


void Timestamp::swap(Timestamp& timestamp)
{
	std::swap(_ts, timestamp._ts);
}


Timestamp Timestamp::fromEpochTime(std::time_t t)
{
	return Timestamp(TimeVal(t)*resolution());
}


Timestamp Timestamp::fromUtcTime(UtcTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}


void Timestamp::update()
{
#if defined(_WIN32)

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart  = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	_ts = ts.QuadPart/10;

#else

	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		throw StringException("cannot get time of day");
	_ts = TimeVal(tv.tv_sec)*resolution() + tv.tv_usec;
	
#endif
}


Timestamp Timestamp::fromFileTime(Timestamp::FileTimeVal fileTime)
{
	return Timestamp((fileTime - FileTimeVal(0x019DB1DE) - 0xD53E8000)/10);
}



} // namespace Utils
