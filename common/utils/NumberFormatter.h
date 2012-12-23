#ifndef NumberFormatter_INCLUDED
#define NumberFormatter_INCLUDED


#include "PreConfig.h"


namespace Utils {


class NumberFormatter
	/// The NumberFormatter class provides static methods
	/// for formatting numeric values into strings.
	///
	/// There are two kind of static member functions:
	///    * format* functions return a std::string containing
	///      the formatted value.
	///    * append* functions append the formatted value to
	///      an existing string.
	///
	/// Internally, std::sprintf() is used to do the actual
	/// formatting.
{
public:
	static std::string format(int value);
		/// Formats an integer value in decimal notation.

	static std::string format(int value, int width);
		/// Formats an integer value in decimal notation,
		/// right justified in a field having at least
		/// the specified width.

	static std::string format0(int value, int width);
		/// Formats an integer value in decimal notation, 
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static std::string formatHex(int value);
		/// Formats an int value in hexadecimal notation.
		/// The value is treated as unsigned.

	static std::string formatHex(int value, int width);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// The value is treated as unsigned.

	static std::string format(unsigned value);
		/// Formats an unsigned int value in decimal notation.

	static std::string format(unsigned value, int width);
		/// Formats an unsigned long int in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static std::string format0(unsigned int value, int width);
		/// Formats an unsigned int value in decimal notation, 
		/// right justified and zero-padded in a field having at 
		/// least the specified width.

	static std::string formatHex(unsigned value);
		/// Formats an unsigned int value in hexadecimal notation.

	static std::string formatHex(unsigned value, int width);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.

	static std::string format(long value);
		/// Formats a long value in decimal notation.

	static std::string format(long value, int width);
		/// Formats a long value in decimal notation,
		/// right justified in a field having at least the 
		/// specified width.

	static std::string format0(long value, int width);
		/// Formats a long value in decimal notation, 
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static std::string formatHex(long value);
		/// Formats an unsigned long value in hexadecimal notation.
		/// The value is treated as unsigned.

	static std::string formatHex(long value, int width);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the 
		/// specified width.
		/// The value is treated as unsigned.

	static std::string format(unsigned long value);
		/// Formats an unsigned long value in decimal notation.

	static std::string format(unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified in a field having at least the specified 
		/// width.

	static std::string format0(unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation, 
		/// right justified and zero-padded
		/// in a field having at least the specified width.

	static std::string formatHex(unsigned long value);
		/// Formats an unsigned long value in hexadecimal notation.

	static std::string formatHex(unsigned long value, int width);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the 
		/// specified width.

#if defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)

	static std::string format(Int64 value);
		/// Formats a 64-bit integer value in decimal notation.

	static std::string format(Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation, 
		/// right justified and zero-padded in a field having at least 
		/// the specified width.

	static std::string formatHex(Int64 value);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// The value is treated as unsigned.

	static std::string formatHex(Int64 value, int width);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least 
		/// the specified width.
		/// The value is treated as unsigned.

	static std::string format(UInt64 value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static std::string format(UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation, 
		/// right justified and zero-padded in a field having at least the 
		/// specified width.

	static std::string formatHex(UInt64 value);
		/// Formats a 64-bit integer value in hexadecimal notation.

	static std::string formatHex(UInt64 value, int width);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least 
		/// the specified width.

#endif // defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)

	static std::string format(float value);
		/// Formats a float value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 8 fractional digits.

	static std::string format(double value);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 16 fractional digits.

	static std::string format(double value, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static std::string format(double value, int width, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static std::string format(const void* ptr);
		/// Formats a pointer in an eight (32-bit architectures) or
		/// sixteen (64-bit architectures) characters wide
		/// field in hexadecimal notation.

	static void append(std::string& str, int value);
		/// Formats an integer value in decimal notation.

	static void append(std::string& str, int value, int width);
		/// Formats an integer value in decimal notation,
		/// right justified in a field having at least
		/// the specified width.

	static void append0(std::string& str, int value, int width);
		/// Formats an integer value in decimal notation, 
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static void appendHex(std::string& str, int value);
		/// Formats an int value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, int value, int width);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, unsigned value);
		/// Formats an unsigned int value in decimal notation.

	static void append(std::string& str, unsigned value, int width);
		/// Formats an unsigned long int in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static void append0(std::string& str, unsigned int value, int width);
		/// Formats an unsigned int value in decimal notation, 
		/// right justified and zero-padded in a field having at 
		/// least the specified width.

	static void appendHex(std::string& str, unsigned value);
		/// Formats an unsigned int value in hexadecimal notation.

	static void appendHex(std::string& str, unsigned value, int width);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.

	static void append(std::string& str, long value);
		/// Formats a long value in decimal notation.

	static void append(std::string& str, long value, int width);
		/// Formats a long value in decimal notation,
		/// right justified in a field having at least the 
		/// specified width.

	static void append0(std::string& str, long value, int width);
		/// Formats a long value in decimal notation, 
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static void appendHex(std::string& str, long value);
		/// Formats an unsigned long value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, long value, int width);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the 
		/// specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, unsigned long value);
		/// Formats an unsigned long value in decimal notation.

	static void append(std::string& str, unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified in a field having at least the specified 
		/// width.

	static void append0(std::string& str, unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation, 
		/// right justified and zero-padded
		/// in a field having at least the specified width.

	static void appendHex(std::string& str, unsigned long value);
		/// Formats an unsigned long value in hexadecimal notation.

	static void appendHex(std::string& str, unsigned long value, int width);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the 
		/// specified width.

#if defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)

	static void append(std::string& str, Int64 value);
		/// Formats a 64-bit integer value in decimal notation.

	static void append(std::string& str, Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation, 
		/// right justified and zero-padded in a field having at least 
		/// the specified width.

	static void appendHex(std::string& str, Int64 value);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, Int64 value, int width);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least 
		/// the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, UInt64 value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static void append(std::string& str, UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation, 
		/// right justified and zero-padded in a field having at least the 
		/// specified width.

	static void appendHex(std::string& str, UInt64 value);
		/// Formats a 64-bit integer value in hexadecimal notation.

	static void appendHex(std::string& str, UInt64 value, int width);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least 
		/// the specified width.

#endif // defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)

	static void append(std::string& str, float value);
		/// Formats a float value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 8 fractional digits.

	static void append(std::string& str, double value);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 16 fractional digits.

	static void append(std::string& str, double value, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static void append(std::string& str, double value, int width, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static void append(std::string& str, const void* ptr);
		/// Formats a pointer in an eight (32-bit architectures) or
		/// sixteen (64-bit architectures) characters wide
		/// field in hexadecimal notation.
};


//
// inlines
//
inline std::string NumberFormatter::format(int value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(int value, int width)
{
	std::string result;
	append(result, value, width);
	return result;
}


inline std::string NumberFormatter::format0(int value, int width)
{
	std::string result;
	append0(result, value, width);
	return result;
}


inline std::string NumberFormatter::formatHex(int value)
{
	std::string result;
	appendHex(result, value);
	return result;
}


inline std::string NumberFormatter::formatHex(int value, int width)
{
	std::string result;
	appendHex(result, value, width);
	return result;
}


inline std::string NumberFormatter::format(unsigned value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(unsigned value, int width)
{
	std::string result;
	append(result, value, width);
	return result;
}


inline std::string NumberFormatter::format0(unsigned int value, int width)
{
	std::string result;
	append0(result, value, width);
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned value)
{
	std::string result;
	appendHex(result, value);
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned value, int width)
{
	std::string result;
	appendHex(result, value, width);
	return result;
}


inline std::string NumberFormatter::format(long value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(long value, int width)
{
	std::string result;
	append(result, value, width);
	return result;
}


inline std::string NumberFormatter::format0(long value, int width)
{
	std::string result;
	append0(result, value, width);
	return result;
}


inline std::string NumberFormatter::formatHex(long value)
{
	std::string result;
	appendHex(result, value);
	return result;
}


inline std::string NumberFormatter::formatHex(long value, int width)
{
	std::string result;
	appendHex(result, value, width);
	return result;
}


inline std::string NumberFormatter::format(unsigned long value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(unsigned long value, int width)
{
	std::string result;
	append(result, value, width);
	return result;
}


inline std::string NumberFormatter::format0(unsigned long value, int width)
{
	std::string result;
	append0(result, value, width);
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned long value)
{
	std::string result;
	appendHex(result, value);
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned long value, int width)
{
	std::string result;
	appendHex(result, value, width);
	return result;
}


#if defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)


inline std::string NumberFormatter::format(Int64 value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(Int64 value, int width)
{
	std::string result;
	append(result, value, width);
	return result;
}


inline std::string NumberFormatter::format0(Int64 value, int width)
{
	std::string result;
	append0(result, value, width);
	return result;
}


inline std::string NumberFormatter::formatHex(Int64 value)
{
	std::string result;
	appendHex(result, value);
	return result;
}


inline std::string NumberFormatter::formatHex(Int64 value, int width)
{
	std::string result;
	appendHex(result, value, width);
	return result;
}


inline std::string NumberFormatter::format(UInt64 value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(UInt64 value, int width)
{
	std::string result;
	append(result, value, width);
	return result;
}


inline std::string NumberFormatter::format0(UInt64 value, int width)
{
	std::string result;
	append0(result, value, width);
	return result;
}


inline std::string NumberFormatter::formatHex(UInt64 value)
{
	std::string result;
	appendHex(result, value);
	return result;
}


inline std::string NumberFormatter::formatHex(UInt64 value, int width)
{
	std::string result;
	appendHex(result, value, width);
	return result;
}


#endif // defined(_HAVE_INT64) && !defined(_LONG_IS_64_BIT)


inline std::string NumberFormatter::format(float value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(double value)
{
	std::string result;
	append(result, value);
	return result;
}


inline std::string NumberFormatter::format(double value, int precision)
{
	std::string result;
	append(result, value, precision);
	return result;
}


inline std::string NumberFormatter::format(double value, int width, int precision)
{
	std::string result;
	append(result, value, width, precision);
	return result;
}


inline std::string NumberFormatter::format(const void* ptr)
{
	std::string result;
	append(result, ptr);
	return result;
}


} // namespace Utils


#endif // NumberFormatter_INCLUDED
