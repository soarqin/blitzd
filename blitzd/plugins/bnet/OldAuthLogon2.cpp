#include "Config.h"
#include "OldAuthLogon2.h"
#include "core/Client.h"
#include "cache/UserCache.h"
#include "crypto/BSHA1Hash.h"
#include "packets/bnet/BNetOldAuthLogon2.h"
#include "packets/bnet/BNetReqEmail.h"
#include "utils/DataConv.h"

namespace Plugins
{
	namespace BNet
	{
		bool OldAuthLogon2::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			byte token[8];
			byte pwd[20];
			std::string username;
			in.read(token, 8);
			in.read(pwd, 20);
			in >> username;
			if(username.length() > 15)
				username.resize(15);

			Packets::BNet::BNetOldAuthLogon2 packet;
			Cache::UserCacheItem::Pointer user = Cache::userCache[username.c_str()];
			if(user.get() == NULL || !user->IsExist())
			{
				packet.result = 1;
			}
			else if((user->GetFullFlag() & UCF_OLSAUTH) == 0)
			{
				packet.result = 6;
				packet.reason = "Your account is not used for this game, please use or register another account.";
			}
			else
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
					packet.result = 0;
					Core::Client& client = (Core::Client&)cl;
					client.SetUser(user);
					client.SetStage(Core::Client::BNET_LOGGEDIN);
					if(user->GetMail().empty())
					{
						Packets::BNet::BNetReqEmail rpacket;
						rpacket.BuildAndSendTo(cl);
					}
				}
				else
					packet.result = 2;
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
