#include "Config.h"
#include "Message.h"
#include "Client.h"
#include "packets/bnet/BNetChatEvent.h"

namespace Core
{
	static bool BuildMessagePacket( Packets::BNet::BNetChatEvent& packet, Client* cl, uint type, std::string msg )
	{
		packet.eventid = type;
		if(cl == NULL)
		{
			packet.uflag = 0;
			packet.ping = 0;
			packet.username = "";
		}
		else
		{
			if(type == Message::Channel)
				packet.uflag = (cl->GetChannel() == NULL) ? 0 : cl->GetChannel()->GetFlag();
			else
				packet.uflag = cl->GetFlag();
			packet.ping = cl->GetLatency();
			packet.username = cl->GetUsername();
		}
		switch (type)
		{
		case Message::AddUser:
		case Message::Join:
		case Message::UserFlags:
			packet.text = (cl == NULL) ? "" : cl->GetStats();
			break;
		default:
			packet.text = msg;
			break;
		}
		return packet.Build();
	}

	bool Message::Send( Client * toconn, Client * conn, uint type, std::string msg )
	{
		if(toconn == NULL)
			return false;
		Packets::BNet::BNetChatEvent packet;
		if(!BuildMessagePacket(packet, conn, type, msg))
			return false;
		packet.SendTo(*toconn);
		if(type == Whisper)
			toconn->SetLastWhisper(conn->GetUsername());
		return true;
	}

	bool Message::Send( std::set<Core::Client *>& toset, Client * conn, uint type, std::string msg, bool ignoreSelf )
	{
		Packets::BNet::BNetChatEvent packet;
		if(!BuildMessagePacket(packet, conn, type, msg))
			return false;
		packet.SendToSet(toset, conn, ignoreSelf);
		if(type == Whisper)
		{
			std::string lwName = conn->GetUsername();
			std::set<Core::Client *>::iterator it;
			for(it = toset.begin(); it != toset.end(); ++ it)
				(*it)->SetLastWhisper(lwName);
		}
		return true;
	}

	bool Message::SendWhisper( std::string toname, Client * conn, std::string msg, bool ack /*= true*/ )
	{
		Client * c = FindOnlineUser(toname, conn, ack);
		if(c == NULL)
			return false;
		if(ack)
		{
			Message::Send(conn, c, WhisperSent, msg);
			if(!c->GetAway().empty())
			{
				Message::Send(conn, NULL, Info, c->GetUsername() + " is away from PC (" + c->GetAway() + ").");
			}
		}
		Message::Send(c, conn, Whisper, msg);
		return true;
	}

	Client * Message::FindOnlineUser( std::string name, Client * conn, bool ack /*= true*/ )
	{
		Client * c = clientPool[name];
		if(c == NULL)
		{
			if(ack)
				Message::Send(conn, NULL, Info, "The user is not logged in!");
			return NULL;
		}
		if(c == conn)
		{
			if(ack)
				Message::Send(conn, NULL, Info, "You cannot do whisper to yourself!");
			return NULL;
		}
		if(!c->GetDND().empty())
		{
			if(ack)
				Message::Send(conn, NULL, Info, c->GetUsername() + " is at DnD status (" + c->GetDND() + ").");
			return NULL;
		}
		return c;
	}
}
