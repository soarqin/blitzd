#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "network/TcpClient.h"
#include "utils/Stream.h"

namespace Core
{
	class Client:
		public Network::TcpClient
	{
	public:
		Client(int);
		virtual ~Client();
	protected:
		virtual void OnRecv(byte *, size_t);
	public:
		inline uint GetSession() {return _session;}
	private:
		void ProcessPacket();
	private:
		Utils::Stream _buffer;

		uint _session;
	};

	class ClientPool
	{
	public:
		ClientPool();
		void Add(Client *);
		void Remove(uint);
		Client * operator[](uint);
		inline uint GetCount() {return (uint)_sessionMap.size();}
	private:
		typedef std::unordered_map<uint, Client *> SessionMap_t;
		SessionMap_t _sessionMap;
	};

	extern ClientPool clientPool;
}

#endif // _CLIENT_H_
