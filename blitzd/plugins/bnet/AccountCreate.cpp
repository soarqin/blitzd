#include "Config.h"
#include "AccountCreate.h"
#include "cache/UserCache.h"
#include "packets/bnet/BNetAccountCreate.h"

namespace Plugins
{
	namespace BNet
	{
		bool AccountCreate::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			byte salt[32], verifier[32];
			std::string username;
			in.read(salt, 32);
			in.read(verifier, 32);
			in >> username;
			if(username.length() > 15)
				username.resize(15);
			Packets::BNet::BNetAccountCreate packet;
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
			user->SetSalt(salt);
			user->SetVerifier(verifier);
			user->AddFlag(UCF_NLSAUTH);
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
