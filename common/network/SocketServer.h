#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_

#include "utils/Thread.h"
#include "utils/Memory.h"
#include "SocketBase.h"

struct event_base;
struct event;

namespace Network
{
    class TcpClient;
	class SocketServer
	{
		friend class _WorkerRunner;
		friend class SocketBase;
		friend class TcpClient;
		friend class UdpSocket;
	public:
		class ITimer
		{
			friend class SocketServer;
		public:
			ITimer(): _event(NULL) {}
			virtual ~ITimer() {if(_event) _MEM.Free(_event);}
			virtual bool operator()() = 0;
		private:
			struct event * _event;
			struct timeval _tv;
		};
	public:
		SocketServer();
		virtual ~SocketServer();
		bool Init(ushort port);
		bool Init(std::vector<ushort> port);
		void Run();
		void Destroy();
		void AddSocket(SocketBase *);
		void RegisterTimer(uint, SocketServer::ITimer *);
	protected:
		virtual TcpClient * OnNewTcpClient(int);
	private:
		struct event_base * _base;
		std::vector<int> _fd_listen;
		std::vector<struct event *> _fd_events;
		Utils::Thread _worker;
		std::map<int, SocketBase::Pointer> _fd_map;

		void Accept(int);
		void Worker();
		void Close(int);
		bool AddListener(ushort);
		static void _listen_cb( int, short, void * );
		static void _timer_cb( int, short, void * );
	};
}

#endif // _SOCKETSERVER_H_
