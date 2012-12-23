#include "Config.h"
#include "WriteUserData.h"
#include "core/Client.h"
#include "cache/UserCache.h"

namespace Plugins
{
	namespace BNet
	{
		bool WriteUserData::Process( Network::TcpClient& cl, Utils::Stream& in )
		{
			uint acount, kcount;
			in >> acount >> kcount;
			std::vector<std::string> anames, knames, kvalues;
			anames.resize(acount);
			knames.resize(kcount);
			kvalues.resize(kcount);
			uint i;
			for(i = 0; i < acount; ++ i)
			{
				in >> anames[i];
				if(anames[i].empty() && i > 0)
					return false;
			}
			for(i = 0; i < kcount; ++ i)
			{
				in >> knames[i];
				if(knames[i].empty() && i < kcount - 1)
					return false;
			}
			for(i = 0; i < kcount; ++ i)
			{
				in >> kvalues[i];
			}
			Core::Client& client = (Core::Client&)cl;
			for(i = 0; i < acount; ++ i)
			{
				Cache::UserCacheItem::Pointer ptr;
				if(anames[i].empty())
					ptr = client.GetUser();
				else
				{
					ptr = Cache::userCache[anames[i]];
				}
				if(ptr.get() == NULL)
					continue;
				for(uint j = 0; j < kcount; ++ j)
				{
					if(!knames[i].empty())
					{
						ptr->SetData(knames[j], kvalues[j]);
					}
				}
			}
			return true;
		}
	}
}
