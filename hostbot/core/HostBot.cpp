#include "Config.h"
#include "BotLoop.h"
#include "Version.h"
#include "network/Network.h"
#include "utils/Thread.h"
#ifdef _WIN32
#include <psapi.h>
#include <TlHelp32.h>

#endif

void RemoveHookingModules()
{
#ifdef _WIN32
	DWORD dwOwnerPID = GetCurrentProcessId();
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
	if( hThreadSnap == INVALID_HANDLE_VALUE )
		return;

	te32.dwSize = sizeof(THREADENTRY32 );

	if( !Thread32First( hThreadSnap, &te32 ) )
	{
		CloseHandle( hThreadSnap );
		return;
	}

	do
	{
		if( te32.th32OwnerProcessID == dwOwnerPID )
		{
			if(te32.th32ThreadID != GetCurrentThreadId())
				TerminateThread(OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID), 0);
		}
	} while( Thread32Next(hThreadSnap, &te32 ) );

	CloseHandle( hThreadSnap );
#endif
}

int main(int argc, char * argv[])
{
	LOG_INFO(("Starting Host Bot Server %d.%d.%d", VER_MAJOR, VER_MINOR, VER_BUILD));
	Network::Init();
	Utils::Thread::sleep(1000);
	RemoveHookingModules();

	HostBot::BotLoop::GetSingleton().Run();

	return 0;
}
