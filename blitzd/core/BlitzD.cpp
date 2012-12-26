#include "Config.h"

#include "network/Network.h"
#include "utils/Thread.h"
#include "DaemonLoop.h"
#include "Version.h"

#ifdef __GNUC__
#ifndef _WIN32
#include <sys/resource.h>
#include <fcntl.h>
extern "C" __attribute__((constructor)) void __serverInit()
{
	struct rlimit rlsrc = {RLIM_INFINITY, RLIM_INFINITY};
	struct rlimit rlsrc2 = {131072, 262144};
	setrlimit(RLIMIT_CORE, &rlsrc);
	setrlimit(RLIMIT_NOFILE, &rlsrc2);
	int f = open("/proc/sys/kernel/core_uses_pid", O_WRONLY);
	if(f < 0)
		return;
	if(write(f, "1", 1) < 0)
		printf("WARNING: Unable to enable core dump with pid!!\n");
	close(f);
}
#endif
#endif


int main(int argc, char * argv[])
{
	LOG_INFO(("Starting Blitz Daemon %d.%d.%d", VER_MAJOR, VER_MINOR, VER_BUILD));
	Network::Init();
	Utils::Thread::sleep(100);

	Core::DaemonLoop::GetSingleton().Run();

	return 0;
}
