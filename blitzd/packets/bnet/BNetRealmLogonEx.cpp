#include "Config.h"
#include "BNetRealmLogonEx.h"
#include "cache/RealmCache.h"
#include "cache/UserCache.h"
#include "crypto/SHA1Hash.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetRealmLogonEx::Pack()
		{
			_packet << cookie;
			Cache::RealmCacheItem::Pointer realm = Cache::realmCache[realmName];
			if(realm.get() == NULL)
			{
				_packet << (uint)0x80000001;
				return true;
			}
			Cache::UserCacheItem::Pointer user = client->GetUser();
			if(user.get() == NULL)
			{
				_packet << (uint)0x80000002;
				return true;
			}
			uint session = client->GetSession();
			uint token = client->GetToken();
			_packet << (uint)0 << (uint)0 << session << (uint)htonl(realm->GetIP()) << (uint)htons(realm->GetPort())/* << token*/;
			_packet.append_zero(12);
			_packet << client->GetGameID() << client->GetVersion();
			_packet.append_zero(8);
			Crypto::SHA1Context context;
			Crypto::SHA1Reset(&context);
			Crypto::SHA1Input(&context, (const byte *)&cookie, 4);
			Crypto::SHA1Input(&context, (const byte *)&session, 4);
			Crypto::SHA1Input(&context, (const byte *)&token, 4);
			Crypto::SHA1Input(&context, user->GetPasswd(), 20);
			byte sha1_result[20];
			Crypto::SHA1Result(&context, sha1_result);
			_packet << sha1_result << user->GetUsername();
			return true;
		}
	}
}
