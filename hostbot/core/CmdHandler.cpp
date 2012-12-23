#include "Config.h"
#include "CmdHandler.h"
#include "MapCfg.h"
#include "BotGameSet.h"
#include "text/Parser.h"

namespace HostBot
{
	CmdHandler::CmdHandler()
	{
		_handlers["exit"] = &CmdHandler::CmdExit;
		_handlers["quit"] = &CmdHandler::CmdExit;
		_handlers["help"] = &CmdHandler::CmdHelp;
		_handlers["?"] = &CmdHandler::CmdHelp;
		_handlers["maplist"] = &CmdHandler::CmdMapList;
		_handlers["maps"] = &CmdHandler::CmdMapList;
		_handlers["gamelist"] = &CmdHandler::CmdGameList;
		_handlers["games"] = &CmdHandler::CmdGameList;
		_handlers["create"] = &CmdHandler::CmdCreate;
		_handlers["cancel"] = &CmdHandler::CmdCancel;
	}

	bool CmdHandler::Process( BotReceiver& rcver, std::string& line )
	{
		std::string cmd;
		size_t off = 0;
		while( off < line.length() && line[off] == ' ' )
			++ off;
		if(off == line.length())
			return true;
		std::vector<std::string> plist;
		size_t pos = line.find(' ', off);
		if(pos == std::string::npos)
		{
			cmd = std::string(line.begin() + off, line.end());
		}
		else
		{
			cmd = std::string(line.begin() + off, line.begin() + pos);
			off = pos + 1;
			Text::SeperateString(plist, line, ' ', off);
		}
		for(size_t i = 0; i < cmd.length(); ++ i)
		{
			cmd[i] = tolower(cmd[i]);
		}
		std::map<std::string, Proc_t>::iterator it = _handlers.find(cmd);
		if(it == _handlers.end())
		{
			return CmdHelp(rcver, plist);
		}
		else
		{
			Proc_t proc = it->second;
			return (this->*proc)(rcver, plist);
		}
	}

	bool CmdHandler::CmdExit( BotReceiver& rcver, StrList_t& params )
	{
		return false;
	}

	bool CmdHandler::CmdHelp( BotReceiver& rcver, StrList_t& params )
	{
		return true;
	}

	bool CmdHandler::CmdMapList( BotReceiver& rcver, StrList_t& params )
	{
		for(size_t i = 0; i < MapCfgPool::GetSingleton().GetCount(); ++ i)
		{
			MapCfg::Pointer ptr = MapCfgPool::GetSingleton()[i];
			char row[1024];
			sprintf(row, "%d. %s  Map: %s  Referee: %s\r\n", 1 + i, ptr->GetAlias().c_str(), ptr->GetFakePath().c_str(), ptr->GetDefaultReferee() ? "yes" : "no");
			rcver.Send(row);
		}
		return true;
	}

	bool CmdHandler::CmdGameList( BotReceiver& rcver, StrList_t& params )
	{
		std::vector<BotGame::Pointer> glist = BotGameSet::GetSingleton().GetGameList();
		size_t c = glist.size();
		for(size_t i = 0; i < c; ++ i)
		{
			char row[1024];
			uint total = glist[i]->GetTotalSlots();
			uint used = glist[i]->GetUsedSlots();
			sprintf(row, "ID: %d  Name: %s  Map: %s  Players: %d/%d  Stage: %s\r\n", glist[i]->GetID(), glist[i]->GetName().c_str(), glist[i]->GetMap().c_str(), used, total, glist[i]->GetStageName().c_str());
			rcver.Send(row);
		}
		return true;
	}

	bool CmdHandler::CmdCreate( BotReceiver& rcver, StrList_t& params )
	{
		if(params.empty())
		{
			rcver.Send("You should at least give game name for creating.\r\n");
			return true;
		}
		std::string gamename = params[0];
		MapCfg::Pointer cfg;
		if(params.size() > 1)
			cfg = MapCfgPool::GetSingleton()[params[1]];
		else
			cfg = MapCfgPool::GetSingleton()[0];
		bool ob = cfg->GetDefaultReferee();
		if(params.size() > 2)
			ob = atoi(params[2].c_str()) > 0;
		BotGameSet::GetSingleton().NewGame(gamename, gamename, cfg, ob);
		char row[1024];
		sprintf(row, "Created game '%s' with map '%s' and referee %s\r\n", gamename.c_str(), cfg->GetFakePath().c_str(), ob ? "on" : "off");
		rcver.Send(row);
		return true;
	}

	bool CmdHandler::CmdCancel( BotReceiver& rcver, StrList_t& params )
	{
		if(params.size() < 1)
		{
			rcver.Send("You should input game id(can be got from command /games) to cancel a game.\r\n");
			return true;
		}

		uint idx = atoi(params[0].c_str());
		BotGame::Pointer ptr = BotGameSet::GetSingleton()[idx];
		if(ptr.get() == NULL)
		{
			rcver.Send("Wrong game id.\r\n");
			return true;
		}
		BotGameSet::GetSingleton().PushToRemoveQueue(ptr->GetID());
		rcver.Send("Removed game.\r\n");
		return true;
	}
}
