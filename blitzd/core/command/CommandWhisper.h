#ifndef _COMMANDWHISPER_H_
#define _COMMANDWHISPER_H_

COMMAND_HANDLER_BEGIN(CommandWhisper, false)
	if(arg1.empty() || argr.empty())
		return;
	Message::SendWhisper(arg1, &client, argr);
COMMAND_HANDLER_END()

#endif // _COMMANDWHISPER_H_
