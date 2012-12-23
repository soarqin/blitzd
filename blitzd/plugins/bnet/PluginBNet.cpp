#include "Config.h"
#include "PluginBNet.h"
#include "AuthInfo.h"
#include "AuthCheck.h"
#include "AccountCreate.h"
#include "AccountChange.h"
#include "AccountChangeProof.h"
#include "AuthLogon.h"
#include "AuthLogonProof.h"
#include "ChangeEmail.h"
#include "OldAccountChange.h"
#include "OldAccountCreate2.h"
#include "OldAuthLogon2.h"
#include "Ping.h"
#include "QueryFileInfo.h"
#include "QueryIcon.h"
#include "ReadUserData.h"
#include "ResetPassword.h"
#include "SetEmail.h"
#include "StopAdv.h"
#include "UDPResponse.h"

namespace Plugins
{
	namespace BNet
	{
		PLUGIN_SUITE_BEGIN(PluginBNet)
			PLUGIN_ADD(AuthInfo)
			PLUGIN_ADD(AuthCheck)
			PLUGIN_ADD(AccountCreate)
			PLUGIN_ADD(AccountChange)
			PLUGIN_ADD(AccountChangeProof)
			PLUGIN_ADD(AuthLogon)
			PLUGIN_ADD(AuthLogonProof)
			PLUGIN_ADD(ChangeEmail)
			PLUGIN_ADD(OldAccountChange)
			PLUGIN_ADD(OldAccountCreate2)
			PLUGIN_ADD(OldAuthLogon2)
			PLUGIN_ADD(Ping)
			PLUGIN_ADD(QueryFileInfo)
			PLUGIN_ADD(QueryIcon)
			PLUGIN_ADD(ReadUserData)
			PLUGIN_ADD(ResetPassword)
			PLUGIN_ADD(SetEmail)
			PLUGIN_ADD(StopAdv)
			PLUGIN_ADD(UDPResponse)
		PLUGIN_SUITE_END()
	}
}
