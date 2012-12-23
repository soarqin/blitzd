#ifndef _COMMANDFRIEND_H_
#define _COMMANDFRIEND_H_

#include "packets/bnet/BNetFriendUpdate.h"
#include "packets/bnet/BNetFriendAdd.h"
#include "packets/bnet/BNetFriendRemove.h"
#include "packets/bnet/BNetFriendPosition.h"

COMMAND_HANDLER_BEGIN(CommandFriend, false)
	if(arg1.empty() || _STR.strcmpi(arg1.c_str(), "h") == 0 || _STR.strcmpi(arg1.c_str(), "help") == 0)
	{
		Core::Message::Send(&client, NULL, Core::Message::Info, "Usage:");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f h          show this help");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f l          list friends");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f w/m <msg>  send whisper to all friends");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f a <name>   add friend");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f r <name>   remove friend");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f p <name>   promote friend");
		Core::Message::Send(&client, NULL, Core::Message::Info, "  /f d <name>   demote friend");
		return;
	}
	else if(_STR.strcmpi(arg1.c_str(), "l") == 0 || _STR.strcmpi(arg1.c_str(), "list") == 0)
	{
		Core::Message::Send(&client, NULL, Core::Message::Info, "Your friends:");
		Cache::UserCacheItem::Pointer user = client.GetUser();
		if(user.get() != NULL)
		{
			for(uint i = 0; i < user->GetFriendCount(); ++ i)
			{
				Cache::UserCacheItem::Friend_t * fr = user->GetFriend(i);
				if(fr != NULL)
				{
					std::string utxt;
					Core::Client * c = Core::clientPool[fr->user->GetUsername()];
					if(c == NULL)
					{
						utxt = "offline";
					}
					else if(c->GetChannel() != NULL)
					{
						if(fr->mutual)
							utxt = "in channel " + c->GetChannel()->GetName();
						else
							utxt = "in a channel";
					}
					else if(c->GetGame() != NULL)
					{
						if(fr->mutual)
							utxt = "in game " + c->GetChannel()->GetName();
						else
							utxt = "in a game";
					}
					else
					{
						utxt = "online";
					}
					Core::Message::Send(&client, NULL, Core::Message::Info, fr->user->GetUsername() + ", " + utxt);
				}
			}
		}
	}
	else if(_STR.strcmpi(arg1.c_str(), "w") == 0 || _STR.strcmpi(arg1.c_str(), "whisper") == 0 || _STR.strcmpi(arg1.c_str(), "m") == 0 || _STR.strcmpi(arg1.c_str(), "msg") == 0)
	{
		if(!argr.empty())
		{
			Cache::UserCacheItem::Pointer user = client.GetUser();
			if(user.get() != NULL)
				user->FriendWhisper(&client, argr, false);
		}
	}
	else if(_STR.strcmpi(arg1.c_str(), "a") == 0 || _STR.strcmpi(arg1.c_str(), "add") == 0)
	{
		Cache::UserCacheItem::Pointer user = Cache::userCache[argr];
		if(user.get() == NULL)
		{
			Core::Message::Send(&client, NULL, Core::Message::Info, "The user does not exist!");
		}
		else if(user == client.GetUser())
		{
			Core::Message::Send(&client, NULL, Core::Message::Info, "Can not add yourself!");
		}
		else
		{
			int mutIdx = -1;
			Cache::UserCacheItem::Pointer cuser = client.GetUser();
			if(cuser.get() == NULL)
				return;
			int idx = cuser->AddFriend(user, &mutIdx);
			if(idx < 0)
			{
				Core::Message::Send(&client, NULL, Core::Message::Info, "Failed to add friend.");
			}
			else if((uint)idx != cuser->GetFriendCount() - 1)
			{
				Core::Message::Send(&client, NULL, Core::Message::Info, "The user is already in your friend list.");
			}
			else
			{
				Core::Client * c = Core::clientPool[user->GetUsername()];
				if(c != NULL)
				{
					if(mutIdx >= 0)
					{
						Packets::BNet::BNetFriendUpdate packet;
						packet.cl = &client;
						packet.index = mutIdx;
						packet.fr = user->GetFriend(mutIdx);
						packet.BuildAndSendTo(*c);
//						UpdateFriend(c, mutIdx, &client, NULL, false);
					}
					Core::Message::Send(c, NULL, Core::Message::Info, cuser->GetUsername() + " added you as his/her friend.");
				}
				Packets::BNet::BNetFriendAdd packet;
				packet.cl = c;
				packet.index = idx;
				packet.fr = cuser->GetFriend(idx);
				packet.BuildAndSendTo(client);
//				UpdateFriend(&client, idx, c, user.username[], true);
				Core::Message::Send(&client, NULL, Core::Message::Info, "Added " + user->GetUsername() + " to your friend list.");
			}
		}
		return;
	}
	else if(_STR.strcmpi(arg1.c_str(), "del") == 0 || _STR.strcmpi(arg1.c_str(), "delete") == 0 || _STR.strcmpi(arg1.c_str(), "r") == 0 || _STR.strcmpi(arg1.c_str(), "remove") == 0)
	{
		Cache::UserCacheItem::Pointer user = Cache::userCache[argr];
		if(user.get() == NULL)
		{
			Core::Message::Send(&client, NULL, Core::Message::Info, "The user does not exist!");
		}
		else
		{
			Cache::UserCacheItem::Pointer cuser = client.GetUser();
			if(cuser.get() == NULL)
				return;
			int idx = cuser->FindFriend(user.get());
			if(idx < 0)
			{
				Core::Message::Send(&client, NULL, Core::Message::Info, "The user is not in your friend list!");
			}
			else
			{
				cuser->RemoveFriend(idx);
				Packets::BNet::BNetFriendRemove packet;
				packet.index = (byte)idx;
				packet.BuildAndSendTo(client);
				Core::Message::Send(&client, NULL, Core::Message::Info, "Removed " + user->GetUsername() + " from your friend list.");
			}
		}
		return;
	}
	else if(_STR.strcmpi(arg1.c_str(), "d") == 0 || _STR.strcmpi(arg1.c_str(), "demote") == 0)
	{
		Cache::UserCacheItem::Pointer user = Cache::userCache[argr];
		if(user.get() == NULL)
		{
			Core::Message::Send(&client, NULL, Core::Message::Info, "The user does not exist!");
		}
		else
		{
			Cache::UserCacheItem::Pointer cuser = client.GetUser();
			if(cuser.get() == NULL)
				return;
			int idx = cuser->FindFriend(user.get());
			if(idx < 0)
			{
				Core::Message::Send(&client, NULL, Core::Message::Info, "The user is not in your friend list!");
			}
			else if((uint)idx + 1 < cuser->GetFriendCount())
			{
				cuser->MoveFriend(idx, idx + 1);
				Packets::BNet::BNetFriendPosition packet;
				packet.oindex = (byte)idx;
				packet.nindex = (byte)idx + 1;
				packet.BuildAndSendTo(client);
			}
		}
		return;
	}
	else if(_STR.strcmpi(arg1.c_str(), "p") == 0 || _STR.strcmpi(arg1.c_str(), "promote") == 0)
	{
		Cache::UserCacheItem::Pointer user = Cache::userCache[argr];
		if(user.get() == NULL)
		{
			Core::Message::Send(&client, NULL, Core::Message::Info, "The user does not exist!");
		}
		else
		{
			Cache::UserCacheItem::Pointer cuser = client.GetUser();
			if(cuser.get() == NULL)
				return;
			int idx = cuser->FindFriend(user.get());
			if(idx < 0)
			{
				Core::Message::Send(&client, NULL, Core::Message::Info, "The user is not in your friend list!");
			}
			else if(idx > 0)
			{
				cuser->MoveFriend(idx, idx - 1);
				Packets::BNet::BNetFriendPosition packet;
				packet.oindex = (byte)idx;
				packet.nindex = (byte)idx - 1;
				packet.BuildAndSendTo(client);
			}
		}
		return;
	}
	return;
COMMAND_HANDLER_END()

#endif // _COMMANDFRIEND_H_
