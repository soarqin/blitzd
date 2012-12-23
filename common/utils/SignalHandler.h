#ifndef SignalHandler_INCLUDED
#define SignalHandler_INCLUDED


#include "OSPlatform.h"


#if defined(UTILS_OS_FAMILY_UNIX)


#include <vector>
#include <setjmp.h>


namespace Utils {


class SignalHandler
{
public:
	SignalHandler();
		/// Creates the SignalHandler.

	~SignalHandler();
		/// Destroys the SignalHandler.

	sigjmp_buf& jumpBuffer();
		/// Returns the top-most sigjmp_buf for the current thread.

	static void throwSignalException(int sig);
		/// Throws a SignalException with a textual description  
		/// of the given signal as argument. 
	
	static void install();
		/// Installs signal handlers for SIGILL, SIGBUS, SIGSEGV
		/// and SIGSYS.

protected:
	static void handleSignal(int sig);
		/// The actual signal handler.

	struct JumpBuffer
		/// sigjmp_buf cannot be used to instantiate a std::vector,
		/// so we provide a wrapper struct.
	{
		sigjmp_buf buf;
	};
	typedef std::vector<JumpBuffer> JumpBufferVec;

	static JumpBufferVec& jumpBufferVec();
		/// Returns the JumpBufferVec for the current thread.

private:
	static JumpBufferVec _jumpBufferVec;
	
	friend class ThreadImpl;
};


#define utils_throw_on_signal \
	utils::SignalHandler _utils_signalHandler; \
	int _utils_signal = sigsetjmp(_utils_signalHandler.jumpBuffer(), 1); \
	if (_utils_signal) _utils_signalHandler.throwSignalException(_utils_signal);


} // namespace Utils


#endif // UTILS_OS_FAMILY_UNIX


#endif // SignalHandler_INCLUDED
