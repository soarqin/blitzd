#include "Config.h"
#include "Command.h"
#include "core/Message.h"

#include "CommandFriend.h"
#include "CommandWhisper.h"
#include "CommandReply.h"
#include "CommandChannels.h"
#include "CommandDND.h"
#include "CommandAway.h"
#include "CommandStatus.h"
#include "CommandJoin.h"
#include "CommandWhoIs.h"

#include <cctype>

namespace Core
{
	namespace Command
	{
		void strtolower(std::string& str)
		{
			size_t len = str.length();
			for(size_t i = 0; i < len; ++ i)
			{
				str[i] = std::tolower(str[i]);
			}
		}
		void Command::Process( Client& cl, std::string text )
		{
			size_t pos = text.find(' ');
			std::string cmd;
			std::string arg1, argr;
			bool havearg = false;
			if(pos == std::string::npos)
			{
				cmd.assign(text.begin() + 1, text.end());
			}
			else
			{
				cmd.assign(text.begin() + 1, text.begin() + pos);
				havearg = true;
			}
			strtolower(cmd);
			std::map<std::string, CommandHandler *>::iterator it = _cmd_map.find(cmd);
			if(it == _cmd_map.end())
				return;
			if(havearg)
			{
				size_t len = text.length();
				do { ++ pos; } while(pos < len && text[pos] == ' ');

				if(pos < len)
				{
					if(it->second->IsSingle())
					{
						arg1.assign(text.begin() + pos, text.end());
					}
					else
					{
						size_t posend = text.find(' ', pos);
						if(posend != std::string::npos)
						{
							arg1.assign(text.begin() + pos, text.begin() + posend);
							pos = posend;
							do { ++ pos; } while(pos < len && text[pos] == ' ');
							if(pos < len)
								argr.assign(text.begin() + pos, text.end());
						}
						else
						{
							arg1.assign(text.begin() + pos, text.end());
						}
					}
				}
			}

			LOG_DEBUG(("Processing %s %s %s", cmd.c_str(), arg1.c_str(), argr.c_str()));
			it->second->Process(cl, arg1, argr);
		}

		void Command::Register( std::string cmd, CommandHandler* ptr )
		{
			_cmd_map[cmd] = ptr;
		}

		Command::Command()
		{
			Register("f", CommandFriend::GetSingletonPtr());
			Register("friend", CommandFriend::GetSingletonPtr());
			Register("w", CommandWhisper::GetSingletonPtr());
			Register("whisper", CommandWhisper::GetSingletonPtr());
			Register("m", CommandWhisper::GetSingletonPtr());
			Register("msg", CommandWhisper::GetSingletonPtr());
			Register("message", CommandWhisper::GetSingletonPtr());
			Register("r", CommandReply::GetSingletonPtr());
			Register("reply", CommandReply::GetSingletonPtr());
			Register("channels", CommandChannels::GetSingletonPtr());
			Register("dnd", CommandDND::GetSingletonPtr());
			Register("away", CommandAway::GetSingletonPtr());
			Register("status", CommandStatus::GetSingletonPtr());
			Register("users", CommandStatus::GetSingletonPtr());
			Register("channel", CommandJoin::GetSingletonPtr());
			Register("join", CommandJoin::GetSingletonPtr());
			Register("whois", CommandWhoIs::GetSingletonPtr());
			Register("where", CommandWhoIs::GetSingletonPtr());
			Register("whereis", CommandWhoIs::GetSingletonPtr());
		}
	}
}
