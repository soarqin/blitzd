#ifndef _COMMANDWHOIS_H_
#define _COMMANDWHOIS_H_

COMMAND_HANDLER_BEGIN(CommandWhoIs, true)
	if(arg1.empty())
		return;
	Core::Client * c = Core::clientPool[arg1];
	if(c == NULL)
	{
		Message::Send(&client, NULL, Message::Info, "The user is offline.");
		return;
	}
	if(c->GetGame() != NULL)
	{
		Message::Send(&client, NULL, Message::Info, c->GetUsername() + " is in game '" + c->GetGame()->GetName() + "'.");
		return;
	}
	if(c->GetChannel() != NULL)
	{
		Message::Send(&client, NULL, Message::Info, c->GetUsername() + " is in channel '" + c->GetChannel()->GetName() + "'.");
		return;
	}
	Message::Send(&client, NULL, Message::Info, c->GetUsername() + " is online.");
COMMAND_HANDLER_END();

#endif // _COMMANDWHOIS_H_
