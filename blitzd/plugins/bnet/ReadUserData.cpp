#include "Config.h"
#include "ReadUserData.h"
#include "core/Client.h"
#include "packets/bnet/BNetReadUserData.h"

namespace Plugins
{
	namespace BNet
	{
		bool ReadUserData::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			Packets::BNet::BNetReadUserData packet;
			in >> packet.acount >> packet.kcount >> packet.token;
			packet.result.resize(packet.acount);
			Core::Client& client = (Core::Client&)cl;
			std::vector<std::string> anames, knames;
			anames.resize(packet.acount);
			knames.resize(packet.kcount);
			uint i;
			for(i = 0; i < packet.acount; ++ i)
			{
				in >> anames[i];
				if(anames[i].empty() && i > 0)
					return false;
			}
			for(i = 0; i < packet.kcount; ++ i)
			{
				in >> knames[i];
				if(knames[i].empty() && i < packet.kcount - 1)
					return false;
			}
			for(i = 0; i < packet.acount; ++ i)
			{
				packet.result[i].resize(packet.kcount);
				Cache::UserCacheItem::Pointer ptr;
				if(anames[i].empty())
					ptr = client.GetUser();
				else
				{
					ptr = Cache::userCache[anames[i]];
				}
				if(ptr.get() == NULL)
					continue;
				for(uint j = 0; j < packet.kcount; ++ j)
				{
					if(!knames[j].empty())
						packet.result[i][j] = ptr->GetData(knames[j]);
				}
			}
			packet.BuildAndSendTo(cl);
			return true;
		}
	}
}
