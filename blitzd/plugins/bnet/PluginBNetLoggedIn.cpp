#include "Config.h"
#include "PluginBNetLoggedIn.h"

#include "CheckAd.h"
#include "ClickAd.h"
#include "DisplayAd.h"
#include "EnterChat.h"
#include "FriendsList.h"
#include "FriendUpdate.h"
#include "GameResult.h"
#include "GetAdvListEx.h"
#include "JoinChannel.h"
#include "LeaveChat.h"
#include "LeaveGame.h"
#include "ListChannel.h"
#include "NetGamePort.h"
#include "NotifyJoin.h"
#include "Null.h"
#include "Ping.h"
#include "Profile.h"
#include "QueryAdUrl.h"
#include "QueryFileInfo.h"
#include "QueryIcon.h"
#include "QueryNews.h"
#include "QueryRealms2.h"
#include "QueryWar3Info.h"
#include "ReadUserData.h"
#include "RealmLogonEx.h"
#include "SendChat.h"
#include "SetEmail.h"
#include "StartAdvEx3.h"
#include "StopAdv.h"
#include "SwitchGameID.h"
#include "UDPResponse.h"
#include "WriteUserData.h"

namespace Plugins
{
	namespace BNet
	{
		PLUGIN_SUITE_BEGIN(PluginBNetLoggedIn)
			PLUGIN_ADD(CheckAd)
			PLUGIN_ADD(ClickAd)
			PLUGIN_ADD(EnterChat)
			PLUGIN_ADD(FriendsList)
			PLUGIN_ADD(FriendUpdate)
			PLUGIN_ADD(GameResult)
			PLUGIN_ADD(GetAdvListEx)
			PLUGIN_ADD(JoinChannel)
			PLUGIN_ADD(LeaveChat)
			PLUGIN_ADD(LeaveGame)
			PLUGIN_ADD(ListChannel)
			PLUGIN_ADD(NetGamePort)
			PLUGIN_ADD(NotifyJoin)
			PLUGIN_ADD(Null)
			PLUGIN_ADD(Ping)
			PLUGIN_ADD(Profile)
			PLUGIN_ADD(QueryAdUrl)
			PLUGIN_ADD(QueryFileInfo)
			PLUGIN_ADD(QueryIcon)
			PLUGIN_ADD(QueryNews)
			PLUGIN_ADD(QueryRealms2)
			PLUGIN_ADD(QueryWar3Info)
			PLUGIN_ADD(ReadUserData)
			PLUGIN_ADD(RealmLogonEx)
			PLUGIN_ADD(SendChat)
			PLUGIN_ADD(SetEmail)
			PLUGIN_ADD(StartAdvEx3)
			PLUGIN_ADD(StopAdv)
			PLUGIN_ADD(SwitchGameID)
			PLUGIN_ADD(UDPResponse)
			PLUGIN_ADD(WriteUserData)
		PLUGIN_SUITE_END()
	}
}
