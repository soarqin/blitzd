#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef WIN32
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4146)
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x500
#endif

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <errno.h>
#ifndef SHUT_WR
#define SHUT_WR 2
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EINPROGRESS WSAEINPROGRESS
#define ECONNRESET WSAECONNRESET
#define ENETRESET WSAENETRESET
#endif
#ifndef ENOMEM
#define EINTR WSAEINTR
#define EFAULT WSAEFAULT
#define ENOMEM WSA_NOT_ENOUGH_MEMORY
#define EAGAIN WSATRY_AGAIN
#endif

#define close(s) closesocket(s)
#ifndef HAVE_SOCKLEN_T
#define HAVE_SOCKLEN_T
typedef int socklen_t;
#endif
#define SOCKET_ERRNO() WSAGetLastError()

#define USE_IOCP

#else

#define SOCKET_ERRNO() (errno)

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#endif

#undef __STRICT_ANSI__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <stack>
#include <set>
#include <string>

#if defined(_MSC_VER) && _MSC_VER < 1500
#include <boost/unordered_map.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
namespace std
{
	using boost::unordered_map;
	using boost::thread;
	using boost::mutex;
}
#else
#include <unordered_map>
#include <thread>
#include <mutex>
#endif

#define ENABLE_LOG
#ifdef _DEBUG
#define LOG_LEVEL 4
#else
#define LOG_NO_VERBOSE
#endif

#ifndef byte
typedef unsigned char byte;
#endif
#ifndef ushort
typedef unsigned short ushort;
#endif
#ifndef uint
typedef unsigned int uint;
#endif
#ifndef uint64
#ifdef _MSC_VER
typedef unsigned __int64 uint64;
#else
typedef unsigned long long uint64;
#endif
#endif
#ifndef int64
#ifdef _MSC_VER
typedef __int64 int64;
#else
typedef long long int64;
#endif
#endif

#include "utils/Log.h"

#endif // _CONFIG_H_
