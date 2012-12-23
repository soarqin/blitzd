#include "Config.h"
#include "TokenReq.h"
#include "core/Client.h"
#include "packets/file/FileToken.h"

namespace Plugins
{
	namespace File
	{
		bool TokenReq::Process( Network::TcpClient& cl, Utils::Stream& st )
		{
			Core::Client& client = (Core::Client&)cl;
			client.SetStage(Core::Client::FILERAW);

			Packets::File::FileToken token;
			token.token = client.GetToken();
			token.BuildAndSendTo(cl);
			return true;
		}
	}
}
