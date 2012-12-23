#include "Config.h"
#include "SetEmail.h"
#include "core/Client.h"
#include "cache/UserCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool SetEmail::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			std::string email;
			in >> email;
			Core::Client& client = (Core::Client&)cl;
			Cache::UserCacheItem::Pointer ptr = client.GetUser();
			if(ptr.get() == NULL)
				return false;
			/* Hook: one space as an empty email */
			if(email.empty())
				email = " ";
			ptr->SetMail(email);
			return true;
		}
	}
}
