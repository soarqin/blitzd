#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>

namespace Utils {
	class StringException:
		public std::exception
	{
	public:
		StringException(const char * _w): _what(_w) {}
		virtual const char * what() const throw() { return _what; }
	private:
		const char * _what;
	};
}

#endif // _EXCEPTION_H_
