#ifndef _SOCKETSERVER_IOCP_H_
#define _SOCKETSERVER_IOCP_H_

#include "utils/Thread.h"
#include "utils/Memory.h"
#include "SocketBase.iocp.h"

namespace Network
{
    class TcpClient;
	class SocketServer
	{
		friend class _WorkerRunner;
		friend class _AccepterRunner;
		friend class SocketBase;
		friend class TcpClient;
		friend class UdpSocket;
	public:
		class ITimer
		{
			friend class SocketServer;
		public:
			ITimer(): _timer(NULL) {}
			virtual ~ITimer()
			{
				if(_timer)
				{
					DeleteTimerQueueTimer(NULL, _timer, NULL);
					_timer = NULL;
				}
			}
			virtual bool operator()() = 0;
			static void CALLBACK TimerProc(PVOID param, BOOLEAN TimerOrWaitFired)
			{
				ITimer * timer = (ITimer *)param;
				if(!(*timer)())
				{
					DeleteTimerQueueTimer(NULL, timer->_timer, NULL);
					timer->_timer = NULL;
				}
			}
		private:
			HANDLE _timer;
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
		HANDLE _hIOCP, _hShutdown;
		std::vector<int> _fd_listen;
		Utils::Thread _worker, _accepter;
		std::map<int, SocketBase::Pointer> _fd_map;

		bool InitIOCP();
		void Accepter();
		void Worker();
		void Close(int);
		bool AddListener(ushort);
		void PushAccept(int);
		static void _listen_cb( int, short, void * );
		static void _timer_cb( int, short, void * );
	};
}
#endif // _SOCKETSERVER_IOCP_H_
