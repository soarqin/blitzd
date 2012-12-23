#ifndef _COMMANDDND_H_
#define _COMMANDDND_H_

COMMAND_HANDLER_BEGIN(CommandDND, true)
	std::string oldDND = client.GetDND();
	client.SetDND(arg1);
	if(arg1.empty())
	{
		if(!oldDND.empty())
			Message::Send(&client, NULL, Message::Info, "You have set your status back to online");
	}
	else
	{
		Message::Send(&client, NULL, Message::Info, "You have set your status to DND (" + arg1 + ").");
	}
COMMAND_HANDLER_END()

#endif // _COMMANDDND_H_
