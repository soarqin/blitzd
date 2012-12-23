#include "Config.h"
#include "AccountChangeProof.h"
#include "core/Client.h"
#include "packets/bnet/BNetAccountChangeProof.h"
#include "cache/UserCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool AccountChangeProof::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			byte clientProof[20];
			in.read(clientProof, 20);
			Packets::BNet::BNetAccountChangeProof packet;
			Cache::UserCacheItem::Pointer ptr = client.GetUser();
			if(ptr.get() != NULL && memcmp(clientProof, client.GetClientProof(), 20) == 0)
			{
				byte s[32], v[32];
				in.read(s, 32);
				in.read(v, 32);
				packet.status = 0;
				memcpy(packet.proof, client.GetServerProof(), 20);
				ptr->SetSalt(s);
				ptr->SetVerifier(v);
			}
			else
			{
				packet.status = 2;
				memset(packet.proof, 0, 20);
			}
			client.SetUser(Cache::UserCacheItem::Pointer());
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
