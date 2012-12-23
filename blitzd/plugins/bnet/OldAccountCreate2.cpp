#include "Config.h"
#include "OldAccountCreate2.h"
#include "cache/UserCache.h"
#include "packets/bnet/BNetOldAccountCreate2.h"

namespace Plugins
{
	namespace BNet
	{
		bool OldAccountCreate2::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			byte pwd[20];
			std::string username;
			in.read(pwd, 20);
			in >> username;
			if(username.length() > 15)
				username.resize(15);
			Packets::BNet::BNetOldAccountCreate2 packet;
			Cache::UserCacheItem::Pointer user = Cache::userCache[username.c_str()];
			if(user.get() != NULL && user->IsExist())
			{
				packet.result = 4;
				packet.BuildAndSendTo(cl);
				return true;
			}
			else
			{
				user = Cache::userCache.Add(username.c_str());
				if(user.get() == NULL)
				{
					packet.result = 8;
					packet.BuildAndSendTo(cl);
					return true;
				}
			}
			packet.result = 0;
			user->SetPasswd(pwd);
			user->AddFlag(UCF_OLSAUTH);
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
