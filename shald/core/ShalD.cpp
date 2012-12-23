#include "Config.h"

#include "network/Network.h"
#include "utils/Thread.h"
#include "DaemonLoop.h"
#include "Version.h"

int main(int argc, char * argv[])
{
	LOG_INFO(("Starting Shal Daemon %d.%d.%d", VER_MAJOR, VER_MINOR, VER_BUILD));
	Network::Init();
	Utils::Thread::sleep(100);

	Core::DaemonLoop::GetSingleton().Run();

	return 0;
}
