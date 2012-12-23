#ifndef _BOTRECEIVER_H_
#define _BOTRECEIVER_H_

#include "network/TcpClient.h"
#include "utils/Stream.h"
#include "utils/Mutex.h"
#include "utils/SharedPtr.h"

namespace HostBot
{
	class BotReceiver:
		public Network::TcpClient,
		public Utils::SharedClass<BotReceiver>
	{
	public:
		BotReceiver(int s);
		virtual ~BotReceiver();
		inline uint GetSession() {return _session;}
		void Send(ushort op, Utils::Stream& buf);
		void Send(ushort op);
		void Send(const char *);
	protected:
		virtual void OnRecv(byte * data, size_t len);
		void ProcessWC3();
		void TelnetNewline();
		bool TelnetReadline(std::string&);
		int IdentifyTelnet();
		void ProcessTelnet();
	private:
		Utils::Stream _buffer;
		uint _session;
		uint _type;
	};
}

#endif // _BOTRECEIVER_H_
