#include "Config.h"
#include "SendChat.h"
#include "core/Client.h"
#include "core/command/Command.h"

namespace Plugins
{
	namespace BNet
	{
		bool SendChat::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;

			std::string text;
			in >> text;
			if(text.empty())
				return false;
			if(text[0] == '/')
			{
				Core::Command::Command::GetSingleton().Process(client, text);
			}
			else
			{
				if(client.GetChannel() == NULL)
					return false;
				client.GetChannel()->Talk(&client, text);
			}
			return true;
		}
	}
}
