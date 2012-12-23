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
			_TcpPort = node->GetAttrInteger("Port", 6202);
			_Name = node->GetAttrString("Name", "Shal Daemon");
			_Description = node->GetAttrString("_Description", "Shal Memory Cache Daemon");
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
