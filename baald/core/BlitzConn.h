#ifndef _BLITZCONN_H_
#define _BLITZCONN_H_

#include "network/TcpClient.h"
#include "utils/Stream.h"
#include "utils/SharedPtr.h"

namespace Core
{
	class BlitzConn:
		public Network::TcpClient
	{
	public:
		static void Init();
		static BlitzConn& Get();
		BlitzConn();
		virtual ~BlitzConn();
	protected:
		virtual void OnRecv(byte *, size_t);
	private:
		Utils::Stream _buffer;

		void ProcessBaal();
		static BlitzConn* _conn;
	};
}

#endif // _BLITZCONN_H_
