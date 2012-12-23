#include "Config.h"
#include "ChangeEmail.h"
#include "cache/UserCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool ChangeEmail::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			std::string username, oldemail, newemail;
			in >> username >> oldemail >> newemail;
			Cache::UserCacheItem::Pointer ptr = Cache::userCache[username];
			if(ptr.get() == NULL)
				return false;
			if(ptr->GetMail() != oldemail)
				return false;
			if(!newemail.empty())
				ptr->SetMail(newemail);
			return true;
		}
	}
}
