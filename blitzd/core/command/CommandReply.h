#ifndef _COMMANDREPLY_H_
#define _COMMANDREPLY_H_

COMMAND_HANDLER_BEGIN(CommandReply, true)
	if(arg1.empty())
		return;
	Message::SendWhisper(client.GetLastWhisper(), &client, arg1);
COMMAND_HANDLER_END()

#endif // _COMMANDREPLY_H_
