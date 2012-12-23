#ifndef _OSPLATFORM_H_
#define _OSPLATFORM_H_

//
// Platform Identification
//
#define UTILS_OS_FREE_BSD      0x0001
#define UTILS_OS_AIX           0x0002
#define UTILS_OS_HPUX          0x0003
#define UTILS_OS_TRU64         0x0004
#define UTILS_OS_LINUX         0x0005
#define UTILS_OS_MAC_OS_X      0x0006
#define UTILS_OS_NET_BSD       0x0007
#define UTILS_OS_OPEN_BSD      0x0008
#define UTILS_OS_IRIX          0x0009
#define UTILS_OS_SOLARIS       0x000a
#define UTILS_OS_QNX           0x000b
#define UTILS_OS_VXWORKS       0x000c
#define UTILS_OS_CYGWIN        0x000d
#define UTILS_OS_UNKNOWN_UNIX  0x00ff
#define UTILS_OS_WINDOWS_NT    0x1001
#define UTILS_OS_WINDOWS_CE    0x1011
#define UTILS_OS_VMS           0x2001

#define UTILS_THREAD_STACK_SIZE	0

#if defined(__FreeBSD__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS_FAMILY_BSD 1
#define UTILS_OS UTILS_OS_FREE_BSD
#elif defined(_AIX) || defined(__TOS_AIX__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_AIX
#elif defined(hpux) || defined(_hpux)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_HPUX
#elif defined(__digital__) || defined(__osf__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_TRU64
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_LINUX
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS_FAMILY_BSD 1
#define UTILS_OS UTILS_OS_MAC_OS_X
#elif defined(__NetBSD__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS_FAMILY_BSD 1
#define UTILS_OS UTILS_OS_NET_BSD
#elif defined(__OpenBSD__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS_FAMILY_BSD 1
#define UTILS_OS UTILS_OS_OPEN_BSD
#elif defined(sgi) || defined(__sgi)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_IRIX
#elif defined(sun) || defined(__sun)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_SOLARIS
#elif defined(__QNX__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_QNX
#elif defined(unix) || defined(__unix) || defined(__unix__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_UNKNOWN_UNIX
#elif defined(_WIN32_WCE)
#define UTILS_OS_FAMILY_WINDOWS 1
#define UTILS_OS UTILS_OS_WINDOWS_CE
#elif defined(_WIN32) || defined(_WIN64)
#define UTILS_OS_FAMILY_WINDOWS 1
#define UTILS_OS UTILS_OS_WINDOWS_NT
#elif defined(__CYGWIN__)
#define UTILS_OS_FAMILY_UNIX 1
#define UTILS_OS UTILS_OS_CYGWIN
#elif defined(__VMS)
#define UTILS_OS_FAMILY_VMS 1
#define UTILS_OS UTILS_OS_VMS
#endif

#endif // _OSPLATFORM_H_

