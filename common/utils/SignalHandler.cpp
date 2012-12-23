#include "Config.h"
#include "PreConfig.h"
#include "SignalHandler.h"
#include "Exception.h"


#if defined(UTILS_OS_FAMILY_UNIX)


#include "Thread.h"
#include "NumberFormatter.h"
#include <cstdlib>
#include <signal.h>


namespace Utils {


SignalHandler::JumpBufferVec SignalHandler::_jumpBufferVec;


SignalHandler::SignalHandler()
{
	JumpBufferVec& jbv = jumpBufferVec();
	JumpBuffer buf;
	jbv.push_back(buf);
}


SignalHandler::~SignalHandler()
{
	jumpBufferVec().pop_back();
}


sigjmp_buf& SignalHandler::jumpBuffer()
{
	return jumpBufferVec().back().buf;
}


void SignalHandler::throwSignalException(int sig)
{
	switch (sig)
	{
	case SIGILL:
		throw StringException("Illegal instruction");
	case SIGBUS:
		throw StringException("Bus error");
	case SIGSEGV:
		throw StringException("Segmentation violation");
	case SIGSYS:
		throw StringException("Invalid system call");
	default:
		throw StringException(NumberFormatter::formatHex(sig).c_str());
	}
}


void SignalHandler::install()
{
	struct sigaction sa;
	sa.sa_handler = handleSignal;
	sa.sa_flags   = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGILL,  &sa, 0);
	sigaction(SIGBUS,  &sa, 0);
	sigaction(SIGSEGV, &sa, 0);
	sigaction(SIGSYS,  &sa, 0);
}


void SignalHandler::handleSignal(int sig)
{
	JumpBufferVec& jb = jumpBufferVec();
	if (!jb.empty())
		siglongjmp(jb.back().buf, sig);
		
	// Abort if no jump buffer registered
	std::abort();
}


SignalHandler::JumpBufferVec& SignalHandler::jumpBufferVec()
{
	ThreadImpl* pThread = ThreadImpl::currentImpl();
	if (pThread)
		return pThread->_jumpBufferVec;
	else
		return _jumpBufferVec;
}


} // namespace Utils


#endif // UTILS_OS_FAMILY_UNIX
