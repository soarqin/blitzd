#include "Config.h"
#include "Cfg.h"
#include "text/XmlNode.h"

namespace Core
{
	Cfg cfg;

	void Cfg::Load( const char * filename )
	{
		LOG_INFO(("Loading config."));
		Text::XmlNode config(filename);

		std::auto_ptr<Text::XmlNode> node(config.GetChild("Server"));
		if(node.get() != NULL)
		{
			_BlitzDAddr = node->GetAttrString("BlitzDAddress", "localhost");
			_BlitzDPort = node->GetAttrInteger("BlitzDPort", 6112);
			_TcpPort = node->GetAttrInteger("Port", 6113);
			_Name = node->GetAttrString("Name", "Baal Realm");
			_Description = node->GetAttrString("_Description", "Diablo II Baal Realm");
		}

		node.reset(config.GetChild("Data"));
		if(node.get() != NULL)
		{
			_HashtableSize = node->GetAttrInteger("hashtableSize", 1024);
		}

		node.reset(config.GetChild("Dir"));
		if(node.get() != NULL)
		{
			_CfgDir = node->GetAttrString("configDir", "config");
		}
	}
}
