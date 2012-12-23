#ifndef _COMMANDSTATUS_H_
#define _COMMANDSTATUS_H_

#include "utils/NumberFormatter.h"
#include "cache/UserCache.h"
#include "cache/GameCache.h"

COMMAND_HANDLER_BEGIN(CommandStatus, true)
	std::string msg = "There are ";
	Utils::NumberFormatter::append(msg, Cache::userCache.GetCount());
	msg += " registered users.";
	Message::Send(&client, NULL, Message::Info, msg);
	msg = "There are ";
	Utils::NumberFormatter::append(msg, clientPool.GetCount());
	msg += " online users in ";
	Utils::NumberFormatter::append(msg, Cache::gameCache.GetCount());
	msg += " games.";
	Message::Send(&client, NULL, Message::Info, msg);
COMMAND_HANDLER_END()


#endif // _COMMANDSTATUS_H_
