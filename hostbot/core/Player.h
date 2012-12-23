#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "utils/Singleton.h"
#include "utils/Stream.h"
#include "utils/SharedPtr.h"
#include "BotReceiver.h"

namespace HostBot
{
	class BotGame;
	class Player:
		public BotReceiver,
		public Utils::SharedClass<Player>
	{
	public:
		Player(int);
		~Player();
		void TimerCheck(uint);
		void LeaveGame();
		void BuildPlayerInfo(Utils::Stream& st, byte id);
		bool SetMapProgress(uint);
		void GlobalChat(std::vector<byte>& toid, byte fromid, std::string& chat);
		void InGameChat(std::vector<byte>& toid, byte fromid, uint flag, std::string& chat);
		void SetTeam(byte);
		void SetColor(byte);
		void SetRace(byte);
		void SetHandicap(byte);
		void LoadEnd();
		void PushAction(const byte *, size_t);
		void PushSync();

		inline void SetHost(BotGame * h) {_host = h;}
		inline void SetPID(uint id) {_pId = id;}
		inline void SetSlotID(uint sid) {_slotId = sid;}
		inline void SetName(std::string& name) {_name = name;}
		inline void SetAddr(const sockaddr_in& addr) {_addr = addr;}
		inline void SetLag(uint l) {_lag = l;}
		inline void SetReady(bool b) {_ready = b;}
		inline void SetLeaveReason(uint r) {_leaveReason = r;}
		inline void SetPing(int p) {_ping = p;}

		inline uint GetPID() {return _pId;}
		inline uint GetSlotID() {return _slotId;}
		inline std::string GetName() {return _name;}
		inline sockaddr_in& GetAddr() {return _addr;}
		inline bool GetLoadEnd() {return _loadend;}
		inline uint GetSyncCounter() {return _syncCounter;}
		inline uint GetLag() {return _lag;}
		inline bool GetReady() {return _ready;}
		inline BotGame * GetHost() {return _host;}
		inline int GetPing() {return _ping;}
	protected:
		BotGame * _host;
		uint _pId;
		uint _slotId;
		std::string _name;
		sockaddr_in _addr;
		uint _lag;
		bool _ready;
		uint _leaveReason;
		uint _pingtick;
		int _ping;

		bool _loadend;
		uint _lastTrigTime;
		uint _syncCounter;

		virtual void OnDisconnect();

	private:
		void ForceLeave();
	};

	class PlayerPool:
		public Utils::Singleton<PlayerPool>
	{
	public:
		void Add(Player *);
		void Remove(uint);
		void TimerCheck(uint);
		Player * operator[] (uint);
	protected:
		std::map<uint, Player * > _players;
		Utils::FastMutex _p_mutex;
	};
}

#endif // _PLAYER_H_
