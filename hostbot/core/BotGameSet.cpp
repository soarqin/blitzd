#include "Config.h"
#include "BotGameSet.h"
#include "BotCfg.h"
#include "MapCfg.h"
#include "utils/ScopedLock.h"

namespace HostBot
{
	uint BotGameSet::curGameid = 1;

	BotGame::Pointer BotGameSet::operator[]( uint id )
	{
		Utils::FastMutex::ScopedLock lk(_locker);
		std::map<uint, BotGame::Pointer>::iterator it = _games.find(id);
		if(it == _games.end())
			return BotGame::Pointer();
		return it->second;
	}

	void BotGameSet::SendUDPInfo( uint ip, ushort port )
	{
		Utils::FastMutex::ScopedLock lk(_locker);
		std::map<uint, BotGame::Pointer>::iterator it;
		for(it = _games.begin(); it != _games.end(); ++ it)
		{
			it->second->SendUDPInfo(ip, port);
		}
	}

	bool BotGameSet::NewGame( std::string gameName, std::string hostName, MapCfg::Pointer cfg, bool isOb )
	{
		Utils::FastMutex::ScopedLock lk(_locker);
		_games[curGameid] = BotGame::Pointer(new BotGame(curGameid, gameName, hostName, cfg, BotGame::MAPSPEED_FAST, BotGame::MAPVIS_DEFAULT, isOb ? BotGame::MAPOBS_REFEREES : BotGame::MAPOBS_NONE));
		LOG_INFO(("Created game: %s", gameName.c_str()));
		++ curGameid;
		return true;
	}

	void BotGameSet::TimerCheck(uint tick)
	{
		Utils::FastMutex::ScopedLock lk(_locker);
		{
			Utils::FastMutex::ScopedLock lk2(_rqlocker);
			size_t count = _removequeue.size();
			if(count > 0)
			{
				for(size_t i = 0; i < count; ++ i)
				{
					_games.erase(_removequeue[i]);
				}
				_removequeue.clear();
			}
		}
		std::map<uint, BotGame::Pointer>::iterator it;
		for(it = _games.begin(); it != _games.end(); ++ it)
		{
			if(it->second.get())
				it->second->TimerCheck(tick);
		}
		if(botCfg.AutoGame() && MapCfgPool::GetSingleton().GetCount() > 0)
		{
			if(_games.size() < botCfg.AutoMaxCount())
			{
				MapCfg::Pointer cfg = MapCfgPool::GetSingleton()[botCfg.AutoDefMap()];
				if(cfg.get() == NULL)
					cfg = MapCfgPool::GetSingleton()[0];
				do
				{
					static int counter = 0;
					++ counter;
					char numstr[128];
					sprintf(numstr, "%d", counter);
					if(!NewGame(botCfg.AutoName() + " #" + numstr, botCfg.AutoName(), cfg, cfg->GetDefaultReferee()))
						break;
				}
				while(_games.size() < botCfg.AutoMaxCount());
			}
		}
	}

	void BotGameSet::RemoveGame( uint id )
	{
		Utils::FastMutex::ScopedLock lk(_locker);
		_games.erase(id);
	}

	std::vector<BotGame::Pointer> BotGameSet::GetGameList()
	{
		Utils::FastMutex::ScopedLock lk(_locker);
		std::vector<BotGame::Pointer> result;
		std::map<uint, BotGame::Pointer>::iterator it;
		for(it = _games.begin(); it != _games.end(); ++ it)
		{
			result.push_back(it->second);
		}
		return result;
	}

	void BotGameSet::PushToRemoveQueue(uint id)
	{
		Utils::FastMutex::ScopedLock lk(_rqlocker);
		_removequeue.push_back(id);
	}
}
