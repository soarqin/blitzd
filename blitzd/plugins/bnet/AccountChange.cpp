#include "Config.h"
#include "AccountChange.h"
#include "core/Client.h"
#include "cache/UserCache.h"
#include "crypto/BnetSRP3.h"
#include "packets/bnet/BNetAccountChange.h"

namespace Plugins
{
	namespace BNet
	{
		bool AccountChange::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Core::Client& client = (Core::Client&)cl;
			byte pubkey[32];
			in.read(pubkey, 32);
			std::string username;
			in >> username;
			if(username.length() > 15)
				username.resize(15);

			Packets::BNet::BNetAccountChange packet;
			LOG_TRACE(("%s is trying to change password...", username.c_str()));
			Cache::UserCacheItem::Pointer ptr = Cache::userCache[username.c_str()];
			if(ptr.get() == NULL/* || Ban.share.checkExpire(user)*/)
			{
				packet.status = 1;
				memset(packet.salt, 0, 32);
				memset(packet.pubkey, 0, 32);
			}
			else
			{
				client.SetUser(ptr);
				packet.status = 0;
				memcpy(packet.salt, ptr->GetSalt(), 32);

				Crypto::BnetSRP3 srp3(username.c_str(), ptr->GetSalt());
				mpz_t server_public_key, client_public_key, verifier, secret;

				mpz_init2(verifier, 256);
				mpz_import(verifier, 32, -1, 1, 0, 0, ptr->GetVerifier());

				mpz_init2(client_public_key, 256);
				mpz_import(client_public_key, 32, -1, 1, 0, 0, pubkey);

				srp3.getServerSessionPublicKey(server_public_key, verifier);
				srp3.getHashedServerSecret(secret, client_public_key, verifier);

				byte client_proof[20], server_proof[20];
				srp3.getClientPasswordProof(client_proof, client_public_key, server_public_key, secret);

				srp3.getServerPasswordProof(server_proof, client_public_key, client_proof, secret);

				client.SetProof(server_proof, client_proof);

				mpz_export(packet.pubkey, NULL, -1, 1, 0, 0, server_public_key);

				mpz_clear(secret);
				mpz_clear(verifier);
				mpz_clear(server_public_key);
				mpz_clear(client_public_key);
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
