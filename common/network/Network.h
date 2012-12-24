#ifndef _NETWORK_H_
#define _NETWORK_H_

namespace Network
{
	bool Init();

	void Cleanup();

	uint ResolveAddress( const char * domain );
}

#endif // _NETWORK_H_
