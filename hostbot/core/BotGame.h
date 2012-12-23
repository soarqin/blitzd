#ifndef _BOTGAME_H_
#define _BOTGAME_H_

#include "utils/SharedPtr.h"
#include "Player.h"
#include "MapCfg.h"

namespace HostBot
{
	class BotGame:
		public Utils::SharedClass<BotGame>
	{
	public:
		enum Stage_t
		{
			STAGE_PREPARE,
			STAGE_COUNTDOWN,
			STAGE_LOADING,
			STAGE_PLAYING,
			STAGE_LAGGING,
			STAGE_FINISHED
		};
		enum MapSpeed_t
		{
			MAPSPEED_SLOW	=		1,
			MAPSPEED_NORMAL	=		2,
			MAPSPEED_FAST	=		3
		};
		enum MapVis_t
		{
			MAPVIS_HIDETERRAIN	=	1,
			MAPVIS_EXPLORED		=	2,
			MAPVIS_ALWAYSVISIBLE=	3,
			MAPVIS_DEFAULT		=	4
		};
		enum MapObs_t
		{
			MAPOBS_NONE		=		1,
			MAPOBS_ONDEFEAT	=		2,
			MAPOBS_ALLOWED	=		3,
			MAPOBS_REFEREES	=		4
		};
		enum MapFlag_t
		{
			MAPFLAG_TEAMSTOGETHER=	1,
			MAPFLAG_FIXEDTEAMS	=	2,
			MAPFLAG_UNITSHARE	=	4,
			MAPFLAG_RANDOMHERO	=	8,
			MAPFLAG_RANDOMRACES	=	16
		};
		BotGame(uint id, std::string&, std::string&, MapCfg::Pointer, MapSpeed_t = MAPSPEED_FAST, MapVis_t = MAPVIS_DEFAULT, MapObs_t = MAPOBS_NONE, uint = 0);
		~BotGame();
		void TimerCheck(uint);
		void SendUDPInfo(uint, ushort);
		bool ApplyNewSlot(Player *, byte);
		void Kick(uint, uint = 7);
		void GetSlotInfo(Utils::Stream& st);
		void SendSlotInfo();
		void SendPlayersInfo(Player&);
		bool SetMapProgress(uint, uint);
		void BuildMapCheck(Utils::Stream& st);
		void SendToAll(ushort op, Utils::Stream& st, byte = 12);
		void SendToAll(ushort op, byte = 12);
		void SendToAll(const byte * buf, size_t len, byte = 12);
		void SendTo(ushort, Utils::Stream&, std::vector<byte>&);
		void SendTo(ushort, std::vector<byte>&);
		void SendTo(const byte * buf, size_t len, std::vector<byte>&);

		void SetTeam(uint, byte);
		void SetColor(uint, byte);
		void SetRace(uint, byte);
		void SetHandicap(uint, byte);
		void SendChat(byte oid, std::vector<byte>& toid, byte fromid, uint flag, std::string& chat);
		void SendChatToAll(std::string chat);
		void SendChatTo(byte toid, std::string chat);
		void LoadEnd(uint);
		void PushAction(byte, const byte *, size_t);

		void GetSlotStatus(uint& used, uint& total);

		inline std::string GetName() {return _gameName;}
		inline Stage_t GetStage() {return _stage;}
		inline uint GetTotalSlots() {return _totalSlots;}
		inline uint GetUsedSlots() {return _usedSlots;}
		inline uint GetID() {return _id;}
		inline std::string GetMap() {return _mapCfg->GetAlias();}
		inline MapCfg::Pointer GetMapCfg() {return _mapCfg;}
		std::string GetStageName();

	protected:
		Utils::FastMutex _playersMutex;
		Utils::FastMutex _slotsMutex;
		Utils::FastMutex _actionsMutex;

	protected:
		Player* _players[12];
		std::vector<byte> _gameInfo;
		std::string _gameName;
		std::string _hostName;
		MapCfg::Pointer _mapCfg;
		uint _id;
		uint _platform;
		uint _version;
		uint _gameType;
		MapSpeed_t _speed;
		MapVis_t _vis;
		MapObs_t _obs;
		uint _flag;
		uint _startTime;
		uint _randomSeed;
		byte _firstPID;

	protected:
		Stage_t _stage;
		uint _totalSlots;
		uint _usedSlots;
		std::vector<GameSlot> _slots;
		uint _lastTrigTime;
		uint _lastLagCheckTime;
		uint _allReadyTime;

		std::vector< std::vector<byte> > _actions;

	protected:
		uint BuildInfoFlag();
		byte CountNoObPlayers();
		void ProcessCommand(byte oid, std::string& cmd);
		void ProcessCommand(byte oid, std::string cmd, std::string arg);
		void SendAllActions(uint);
		void CheckPlayersLag();
		void SendStartLag();
		void SendStopLag(Player *);
		void SendStopLag(byte id, uint tick);
		void DeCreate();
		void ReCalcUsedSlots();
		void SendStartGame();
		void CheckForReady();
		void SendNumPlayerInfo();
		void SendWelcome(uint);
		void BroadcastSlotInfo();
	};
}

#endif // _BOTGAME_H_
