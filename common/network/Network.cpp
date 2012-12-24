#include "Config.h"
#include "Network.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include <event2/thread.h>

namespace Network
{

	bool Init()
	{
#ifdef _WIN32
		WSADATA wsaData;
		if( 0 != WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) )
			return false;
		evthread_use_windows_threads();
#else
		evthread_use_pthreads();
#endif
		return true;
	}

	void Cleanup()
	{
#ifdef _WIN32
		WSACleanup();
#endif
	}

	uint ResolveAddress( const char * domain )
	{
		struct hostent * remoteHost;
		if (isalpha(domain[0])) {
			remoteHost = gethostbyname(domain);
			if(remoteHost == NULL)
				return (uint)-1;
			return ntohl(*(uint *)remoteHost->h_addr_list[0]);
		} else {
			return ntohl(inet_addr(domain));
		}
	}

}
