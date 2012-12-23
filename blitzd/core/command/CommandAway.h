#ifndef _COMMANDAWAY_H_
#define _COMMANDAWAY_H_

COMMAND_HANDLER_BEGIN(CommandAway, true)
	std::string oldAway = client.GetAway();
	client.SetAway(arg1);
	if(arg1.empty())
	{
		if(!oldAway.empty())
			Message::Send(&client, NULL, Message::Info, "You have set your status back to online");
	}
	else
	{
		Message::Send(&client, NULL, Message::Info, "You have set your status to away (" + arg1 + ").");
	}
COMMAND_HANDLER_END()

#endif // _COMMANDAWAY_H_
