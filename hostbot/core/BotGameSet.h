#ifndef _BOTGAMESET_H_
#define _BOTGAMESET_H_

#include "BotGame.h"
#include "utils/Singleton.h"
#include "utils/Mutex.h"

namespace HostBot
{
	class BotGameSet:
		public Utils::Singleton<BotGameSet>
	{
	public:
		BotGame::Pointer operator[](uint);
		void TimerCheck(uint);
		void SendUDPInfo(uint, ushort);
		bool NewGame(std::string, std::string, MapCfg::Pointer, bool isOb);
		void RemoveGame(uint);
		std::vector<BotGame::Pointer> GetGameList();
		void PushToRemoveQueue(uint);
	protected:
		static uint curGameid;
		std::map<uint, BotGame::Pointer> _games;
		std::vector<uint> _removequeue;
		Utils::FastMutex _locker;
		Utils::FastMutex _rqlocker;
	};
}

#endif // _BOTGAMESET_H_
