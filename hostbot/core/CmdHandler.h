#ifndef _CMDHANDLER_H_
#define _CMDHANDLER_H_

#include "BotReceiver.h"
#include "utils/Singleton.h"

namespace HostBot
{
	class CmdHandler:
		public Utils::Singleton<CmdHandler>
	{
	public:
		typedef std::vector<std::string> StrList_t;
		typedef bool (CmdHandler::*Proc_t)(BotReceiver& rcver, StrList_t& params);
		CmdHandler();
		bool Process(BotReceiver& rcver, std::string& line);
	protected:
		bool CmdExit(BotReceiver& rcver, StrList_t& params);
		bool CmdHelp(BotReceiver& rcver, StrList_t& params);
		bool CmdMapList(BotReceiver& rcver, StrList_t& params);
		bool CmdGameList(BotReceiver& rcver, StrList_t& params);
		bool CmdCreate(BotReceiver& rcver, StrList_t& params);
		bool CmdCancel(BotReceiver& rcver, StrList_t& params);
		std::map<std::string, Proc_t> _handlers;
	};
}

#endif // _CMDHANDLER_H_
