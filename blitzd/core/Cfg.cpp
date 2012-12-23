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
			_TcpPort = node->GetAttrInteger("Port", 6112);
			_UdpPort = node->GetAttrInteger("UDPPort", 6112);
			_Name = node->GetAttrString("Name", "Blitz Daemon Server");
		}

		node.reset(config.GetChild("Data"));
		if(node.get() != NULL)
		{
			_DBEngine = node->GetAttrString("DbEngine", "sqlite");
			_DBFile = node->GetAttrString("Database", "blitzd.sqlite");
			_HashtableSize = node->GetAttrInteger("HashtableSize", 1024);
		}

		node.reset(config.GetChild("Dir"));
		if(node.get() != NULL)
		{
			_CfgDir = node->GetAttrString("ConfigDir", "config");
			_FilesDir = node->GetAttrString("FilesDir", "files");
			_GamesDir = node->GetAttrString("GamesDir", "games");
			_DataDir = node->GetAttrString("Datadir", "data");
		}

		node.reset(config.GetChild("File"));
		if(node.get() != NULL)
		{
			_NewsFile = node->GetAttrString("News", "news.txt");
			_MotdFile = node->GetAttrString("Motd", "motd.txt");
			_CheckRevFile = node->GetAttrString("Checkrev", "checkrev.xml");
			_ChannelFile = node->GetAttrString("Channel", "channel.xml");
			_LadderMapFile = node->GetAttrString("LadderMap", "laddermap.xml");
		}
	}
}
