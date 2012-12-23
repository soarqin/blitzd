#include "Config.h"
#include "AuthLogonProof.h"
#include "core/Client.h"
#include "cache/UserCache.h"
#include "packets/bnet/BNetAuthLogonProof.h"

namespace Plugins
{
	namespace BNet
	{
		bool AuthLogonProof::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			byte clientProof[20];
			in.read(clientProof, 20);
			Packets::BNet::BNetAuthLogonProof packet;
			if(memcmp(clientProof, client.GetClientProof(), 20) == 0)
			{
				Cache::UserCacheItem::Pointer ptr = client.GetUser();
				if(ptr.get() != NULL && ptr->GetMail().empty())
					packet.status = 0x0E;
				else
					packet.status = 0;
				client.SetStage(Core::Client::BNET_LOGGEDIN);
				memcpy(packet.proof, client.GetServerProof(), 20);
			}
			else
			{
				packet.status = 2;
				memset(packet.proof, 0, 20);
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
