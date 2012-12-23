#ifndef _MESSAGE_H_
#define _MESSAGE_H_

namespace Core
{
	class Client;
	class Message
	{
	public:
		enum
		{
			AddUser = 1,
			Join = 2,
			Leave = 3,
			Whisper = 4,
			Talk = 5,
			Broadcast = 6,
			Channel = 7,
			UserFlags = 9,
			WhisperSent = 0xA,
			ChannelFull = 0xD,
			ChannelNotExit = 0xE,
			ChannelRestricted = 0xF,
			Info = 0x12,
			Error = 0x13,
			Emote = 0x17
		};
	public:
		static bool Send(Client * toconn, Client * conn, uint type, std::string msg);
		static bool Send(std::set<Core::Client *>& toset, Client * conn, uint type, std::string msg, bool ignoreSelf = true);
		static bool SendWhisper(std::string toname, Client * conn, std::string msg, bool ack = true);
	private:
		static Client * FindOnlineUser(std::string name, Client * conn, bool ack = true);
	};
}

#endif // _MESSAGE_H_
