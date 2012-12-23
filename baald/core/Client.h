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
		enum Stage_t
		{
			CONNECTED,
			BNETREALM,
			D2GS,
		};
	public:
		Client(int);
		virtual ~Client();
	protected:
		virtual void OnRecv(byte *, size_t);
	public:
		inline Stage_t GetStage() {return _stage;}
		inline void SetStage(Stage_t s) {_stage = s;}
		inline uint GetSession() {return _session;}
	private:
		void ProcessBNetRealm();
	private:
		Utils::Stream _buffer;

		Stage_t _stage;
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
