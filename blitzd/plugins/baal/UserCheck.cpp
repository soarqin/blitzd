#include "Config.h"
#include "UserCheck.h"
#include "core/Client.h"
#include "packets/baal/BaalCheckReply.h"
#include "crypto/SHA1Hash.h"

namespace Plugins
{
	namespace Baal
	{
		bool UserCheck::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			uint remote_token, cookie;
			byte hash[20];
			std::string name;
			in >> remote_token >> cookie >> hash >> name;
			Core::Client * client = Core::clientPool[name];
			Packets::Baal::BaalCheckReply packet(remote_token);
			if(client == NULL || client->GetUser().get() == NULL)
			{
				packet.result = 0x0C;
			}
			else
			{
				uint session = client->GetSession();
				uint token = client->GetToken();
				Crypto::SHA1Context context;
				Crypto::SHA1Reset(&context);
				Crypto::SHA1Input(&context, (const byte *)&cookie, 4);
				Crypto::SHA1Input(&context, (const byte *)&session, 4);
				Crypto::SHA1Input(&context, (const byte *)&token, 4);
				Crypto::SHA1Input(&context, client->GetUser()->GetPasswd(), 20);
				byte sha1_result[20];
				Crypto::SHA1Result(&context, sha1_result);

				packet.result = (memcmp(sha1_result, hash, 20) == 0) ? 0 : 0x0C;
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
