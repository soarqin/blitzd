#include "Config.h"
#include "BotCfg.h"
#include "MapCfg.h"
#include "text/XmlNode.h"

namespace HostBot
{
	BotCfg botCfg;
	void BotCfg::Load( const char * filename )
	{
		LOG_INFO(("Loading config."));
		Text::XmlNode config(filename);

		std::auto_ptr<Text::XmlNode> node(config.GetChild("Admin"));
		if(node.get() != NULL)
		{
			_password = node->GetAttrString("Password", "12345");
		}

		node.reset(config.GetChild("Game"));
		if(node.get() != NULL)
		{
			_saveReplay = node->GetAttrInteger("SaveReplay", 0) > 0;
			_syncLimit = node->GetAttrInteger("SyncLimit", 40);
			_gameVersion = node->GetAttrInteger("Version", 24);
		}

		node.reset(config.GetChild("Auto"));
		if(node.get() != NULL)
		{
			_autoGame = node->GetAttrInteger("Enable", 0) > 0;
			_autoName = node->GetAttrString("Name", "Host Bot");
			_autoMaxCount = node->GetAttrInteger("MaxCount", 5);
			_autoDefMap = node->GetAttrString("DefaultMap", "");
		}

		node.reset(config.GetChild("Network"));
		if(node.get() != NULL)
		{
			_tcpPort = node->GetAttrInteger("TcpPort", 6112);
			_udpPort = node->GetAttrInteger("UdpPort", 6112);
		}

		node.reset(config.GetChild("Scripts"));
		if(node.get() != NULL)
		{
			_scriptPath = node->GetAttrString("Path", "Scripts");
			if(!_scriptPath.empty() && _scriptPath[_scriptPath.size() - 1] != '\\')
				_scriptPath += '\\';
		}

		node.reset(config.GetChild("Maps"));
		if(node.get() != NULL)
		{
			std::vector<std::vector<std::string> > mapcfgset;
			std::vector<std::string> keys;
			keys.push_back("Name");
			keys.push_back("Path");
			keys.push_back("FakePath");
			keys.push_back("MinStartCount");
			keys.push_back("Referee");
			node->ReadArray(mapcfgset, keys, "Map");
			for(size_t i = 0; i < mapcfgset.size(); ++ i)
			{
				MapCfgPool::GetSingleton().Add(MapCfg::Pointer(new MapCfg(mapcfgset[i][0].c_str(), mapcfgset[i][1].c_str(), mapcfgset[i][2].c_str(), atoi(mapcfgset[i][3].c_str()), atoi(mapcfgset[i][4].c_str()) > 0)));
			}
		}
	}
}
