#include "Config.h"
#include "OldAccountChange.h"
#include "cache/UserCache.h"
#include "crypto/BSHA1Hash.h"
#include "packets/bnet/BNetOldAccountChange.h"
#include "utils/DataConv.h"

namespace Plugins
{
	namespace BNet
	{
		bool OldAccountChange::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			byte token[8];
			byte pwd[20], newpwd[20];
			std::string username;
			in.read(token, 8);
			in.read(pwd, 20);
			in.read(newpwd, 20);
			in >> username;
			if(username.length() > 15)
				username.resize(15);

			Packets::BNet::BNetOldAccountChange packet;
			Cache::UserCacheItem::Pointer user = Cache::userCache[username.c_str()];
			if(user.get() != NULL && user->IsExist())
			{
				byte cres[20];
				Crypto::BSHA1Context bsha1;
				BSHA1Reset(&bsha1);
				BSHA1Input(&bsha1, token, 8);
				BSHA1Input(&bsha1, user->GetPasswd(), 20);
				BSHA1Result(&bsha1, cres);
				for(int i = 0; i < 5; ++ i)
				{
					((uint *)cres)[i] = Utils::Reverse(((uint *)cres)[i]);
				}
				if(memcmp(cres, pwd, 20) == 0)
				{
					packet.result = 1;
					user->SetPasswd(newpwd);
				}
				else
					packet.result = 0;
			}
			else
			{
				packet.result = 0;
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
