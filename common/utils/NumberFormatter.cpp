#include "Config.h"
#include "NumberFormatter.h"
#include <cstdio>
#include <cctype>
#include <cassert>


#if defined(_MSC_VER)
	#define I64_FMT "I64"
#elif defined(__APPLE__) 
	#define I64_FMT "q"
#else
	#define I64_FMT "ll"
#endif


namespace Utils {


void NumberFormatter::append(std::string& str, int value)
{
	char buffer[64];
	std::sprintf(buffer, "%d", value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, int value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%*d", width, value);
	str.append(buffer);
}


void NumberFormatter::append0(std::string& str, int value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*d", width, value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, int value)
{
	char buffer[64];
	std::sprintf(buffer, "%X", value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, int value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*X", width, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, unsigned value)
{
	char buffer[64];
	std::sprintf(buffer, "%u", value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, unsigned value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%*u", width, value);
	str.append(buffer);
}


void NumberFormatter::append0(std::string& str, unsigned int value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*u", width, value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, unsigned value)
{
	char buffer[64];
	std::sprintf(buffer, "%X", value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, unsigned value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*X", width, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, long value)
{
	char buffer[64];
	std::sprintf(buffer, "%ld", value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, long value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%*ld", width, value);
	str.append(buffer);
}


void NumberFormatter::append0(std::string& str, long value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*ld", width, value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, long value)
{
	char buffer[64];
	std::sprintf(buffer, "%lX", value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, long value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*lX", width, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, unsigned long value)
{
	char buffer[64];
	std::sprintf(buffer, "%lu", value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, unsigned long value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%*lu", width, value);
	str.append(buffer);
}


void NumberFormatter::append0(std::string& str, unsigned long value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*lu", width, value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value)
{
	char buffer[64];
	std::sprintf(buffer, "%lX", value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*lX", width, value);
	str.append(buffer);
}


#if defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)


void NumberFormatter::append(std::string& str, Int64 value)
{
	char buffer[64];
	std::sprintf(buffer, "%"I64_FMT"d", value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, Int64 value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%*"I64_FMT"d", width, value);
	str.append(buffer);
}


void NumberFormatter::append0(std::string& str, Int64 value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*"I64_FMT"d", width, value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, Int64 value)
{
	char buffer[64];
	std::sprintf(buffer, "%"I64_FMT"X", value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, Int64 value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*"I64_FMT"X", width, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, UInt64 value)
{
	char buffer[64];
	std::sprintf(buffer, "%"I64_FMT"u", value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, UInt64 value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%*"I64_FMT"u", width, value);
	str.append(buffer);
}


void NumberFormatter::append0(std::string& str, UInt64 value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*"I64_FMT"u", width, value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value)
{
	char buffer[64];
	std::sprintf(buffer, "%"I64_FMT"X", value);
	str.append(buffer);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value, int width)
{
	assert (width > 0 && width < 64);

	char buffer[64];
	std::sprintf(buffer, "%0*"I64_FMT"X", width, value);
	str.append(buffer);
}


#endif // defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)


void NumberFormatter::append(std::string& str, float value)
{
	char buffer[64];
	std::sprintf(buffer, "%.*g", 8, (double) value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value)
{
	char buffer[64];
	std::sprintf(buffer, "%.*g", 16, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int precision)
{
	assert (precision >= 0 && precision < 32);

	char buffer[64];
	std::sprintf(buffer, "%.*f", precision, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int width, int precision)
{
	assert (width > 0 && width < 64 && precision >= 0 && precision < width);

	char buffer[64];
	std::sprintf(buffer, "%*.*f", width, precision, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, const void* ptr)
{
	char buffer[24];
#if defined(POCO_PTR_IS_64_BIT)
	std::sprintf(buffer, "%016"I64_FMT"X", (UIntPtr) ptr);
#else
	std::sprintf(buffer, "%08lX", (UIntPtr) ptr);
#endif
	str.append(buffer);
}


} // namespace Utils
