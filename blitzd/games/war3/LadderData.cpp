#include "Config.h"
#include "LadderData.h"
#include "utils/Memory.h"
#include "utils/Stream.h"

#include "text/XmlNode.h"
#include "core/Cfg.h"

namespace Games
{
	namespace War3
	{
		static const uint mi_min[4] = {0, 12, 17, 18};
		static byte mapinfo_prefix_template[18][5] = {                          /*queue */
			/* PG 1v1       */ {0x00, 0x00, 0x03, 0x3F, 0x00}, 	/*  0   */
			/* PG 2v2       */ {0x01, 0x00, 0x02, 0x3F, 0x00},	/*  1   */
			/* PG 3v3       */ {0x02, 0x00, 0x01, 0x3F, 0x00},	/*  2   */
			/* PG 4v4       */ {0x03, 0x00, 0x01, 0x3F, 0x00},	/*  3   */
			/* PG 5v5       */ {0x05, 0x00, 0x01, 0x3F, 0x00}, 	/*  4   */
			/* PG 6v6       */ {0x06, 0x00, 0x01, 0x3F, 0x00},	/*  5   */
			/* PG 2v2v2     */ {0x07, 0x00, 0x01, 0x3F, 0x00},	/*  6   */
			/* PG 3v3v3     */ {0x08, 0x00, 0x01, 0x3F, 0x00},	/*  7   */
			/* PG 4v4v4     */ {0x09, 0x00, 0x01, 0x3F, 0x00},	/*  8   */
			/* PG 2v2v2v2   */ {0x0A, 0x00, 0x01, 0x3F, 0x00},	/*  9   */
			/* PG 3v3v3v3   */ {0x0B, 0x00, 0x01, 0x3F, 0x00},	/* 10   */
			/* PG sffa      */ {0x04, 0x00, 0x02, 0x3F, 0x00},	/* 11   */

			/* AT 2v2       */ {0x00, 0x00, 0x02, 0x3F, 0x02},	/* 12   */
			/* AT 3v3       */ {0x02, 0x00, 0x02, 0x3F, 0x03},	/* 13   */
			/* AT 4v4       */ {0x03, 0x00, 0x02, 0x3F, 0x04},	/* 14   */
			/* AT 2v2v2     */ {0x04, 0x00, 0x02, 0x3F, 0x02},	/* 15   */
			/* AT tffa      */ {0x01, 0x00, 0x02, 0x3F, 0x02},	/* 16   */

			/* TY           */ {0x00, 0x01, 0x00, 0x3F, 0x00}   /* 17   */
		};
		static const char* ladder_shortname[] = {
			"SOLO",
			"TEAM",
			"FFA ",
			"2VS2",
			"3VS3",
			"4VS4",
			"CLNS",
			"CLN2",
			"CLN3",
			"CLN4"
		};
		static const char* page_name[] = {
			"server",
			"player",
			"tourney",
			"clan"
		};


		LadderData::LadderData()
		{
			memcpy(mapinfo_prefix, mapinfo_prefix_template, sizeof(mapinfo_prefix_template));
		}

		void LadderData::Load()
		{
			Text::XmlNode xml((Core::cfg.GetCfgDir() + '/' + Core::cfg.GetLadderMapFile()).c_str());
			std::auto_ptr<Text::XmlNode> node(xml.GetChild("Mapsets"));
			if(node.get() != NULL)
			{
				std::auto_ptr<Text::XmlNode> child(node->GetChild("Mapset"));
				while(child.get() != NULL)
				{
					const char * typestr = child->GetAttrString("type", "");
					uint type = 0;
					if(_STR.strcmpi(typestr, "PG") == 0)
					{
						type = 0;
					}
					else if(_STR.strcmpi(typestr, "AT") == 0)
					{
						type = 1;
					}
					else if(_STR.strcmpi(typestr, "TOURNEY") == 0)
					{
						type = 2;
					}
					else
					{
						child.reset(child->GetNext("Mapset"));
						continue;
					}
					std::auto_ptr<Text::XmlNode> child2(child->GetChild("Maps"));
					while(child2.get() != NULL)
					{
						typestr = child2->GetAttrString("type", "");
						const char* mapinfo_tag[] = {
							"1vs1",
							"2vs2",
							"3vs3",
							"4vs4",
							"5vs5",
							"6vs6",
							"2vs2vs2",
							"3vs3vs3",
							"4vs4vs4",
							"2vs2vs2vs2",
							"3vs3vs3vs3",
							"ffa",
							"2vs2",
							"3vs3",
							"4vs4",
							"2vs2vs2",
							"ffa",
							"tourney",
							NULL
						};
						byte mtype = 0xff;
						for(uint i = mi_min[type]; i < mi_min[type + 1]; i ++)
						{
							if(_STR.strcmpi(mapinfo_tag[i], typestr) == 0)
							{
								mtype = (byte)i;
								break;
							}
						}
						if(mtype == 0xff)
						{
							child2.reset(child2->GetNext("Maps"));
							continue;
						}
						map_desc[mtype] = child2->GetAttrString("desc", "");
						map_longDesc[mtype] = child2->GetAttrString("longDesc", "");
						mapinfo_prefix[mtype][2] = (byte)child2->GetAttrInteger("thumbsDown", 0);

						std::vector<std::vector<std::string> > result;
						std::vector<std::string> keys;
						keys.push_back("path");
						child2->ReadArray(result, keys, "Map");
						for(size_t z = 0; z < result.size(); ++ z)
						{
							__AddMap(result[z][0], mtype);
						}
						child2.reset(child2->GetNext("Maps"));
					}
					child.reset(child->GetNext("Mapset"));
				}
			}
			node.reset(xml.GetChild("Ladders"));
			if(node.get() != NULL)
			{
				std::vector<std::vector<std::string> > result;
				std::vector<std::string> keys;
				keys.push_back("type");
				keys.push_back("desc");
				keys.push_back("url");
				node->ReadArray(result, keys, "Ladder");
				for(size_t z = 0; z < result.size(); ++ z)
				{
					int idx = -1;
					for(int i = 0; i < 10; i ++)
					{
						if(_STR.strcmpi(result[z][0].c_str(), ladder_shortname[i]) == 0)
						{
							idx = i;
							break;
						}
					}
					if(idx < 0)
						continue;
					ladder_desc[idx] = result[z][1];
					ladder_url[idx] = result[z][2];
				}
			}
			node.reset(xml.GetChild("Pages"));
			if(node.get() != NULL)
			{
				std::vector<std::vector<std::string> > result;
				std::vector<std::string> keys;
				keys.push_back("type");
				keys.push_back("url");
				node->ReadArray(result, keys, "Page");
				for(size_t z = 0; z < result.size(); ++ z)
				{
					int idx = -1;
					for(int i = 0; i < 4; i ++)
					{
						if(_STR.strcmpi(result[z][0].c_str(), page_name[i]) == 0)
						{
							idx = i;
							break;
						}
					}
					if(idx < 0)
						continue;
					page_url[idx] = result[z][1];
				}
			}
		}

		std::vector<byte> LadderData::GetUrlData()
		{
			Utils::Stream st;
			for(int i = 0; i < 4; ++ i)
			{
				st << page_url[i];
			}
			return st;
		}

		std::vector<byte> LadderData::GetMapName()
		{
			Utils::Stream st;
			st << (byte)_maplist.size();
			for(size_t i = 0; i < _maplist.size(); i ++)
			{
				st << _maplist[i];
			}
			return st;
		}

		std::vector<byte> LadderData::GetMapInfo()
		{
			Utils::Stream st;
			st << (byte)0;
			byte c = 0;
			for(byte z = 0; z < 3; z ++)
			{
				std::vector<std::vector<byte> > mapinfo;
				for(uint i = mi_min[z]; i < _mapindex.size() && i < mi_min[z + 1]; ++ i)
				{
					if(_mapindex[i].size() == 0)
						continue;
					Utils::Stream p;
					p.append(mapinfo_prefix[i], 5);
					p << (byte)_mapindex[i].size() << _mapindex[i];
					mapinfo.push_back(p);
				}
				if(mapinfo.size() == 0)
					continue;
				st << z << (byte)mapinfo.size();
				for(size_t i = 0; i < mapinfo.size(); ++ i)
				{
					st.append(&mapinfo[i][0], mapinfo[i].size());
				}
				c ++;
			}
			st[0] = c;
			return st;
		}

		std::vector<byte> LadderData::GetMapDesc()
		{
			const char* defaultDesc[] = {
				"One vs. One",
				"Two vs. Two",
				"Three vs. Three",
				"Four vs. Four",
				"Five vs. Five",
				"Six Vs. Six",
				"Two vs. Two vs. Two",
				"Three vs. Three vs. Three",
				"Four vs. Four vs. Four",
				"Two vs. Two vs. Two vs. Two",
				"Three vs. Three vs. Three vs. Three",
				"Small Free for All",
				"Two vs. Two",
				"Three vs. Three",
				"Four vs. Four",
				"Two vs. Two vs. Two",
				"Team Free for All",
				"Tournament Game"
			};
			const char* defaultDescLong[] = {
				"Two players fight to the death",
				"Two teams of two vie for dominance",
				"Two teams of three face off on the battlefield",
				"Two teams of four head to battle",
				"Two teams of five - who will prevail?",
				"Two teams of six - get ready to rumble!",
				"Three teams of two, can you handle it?",
				"Three teams of three battle each other",
				"Three teams of four - things getting crowded?",
				"Four teams of two, is this a challenge?",
				"Four teams of three, the ultimate challenge!",
				"Can you defeat 3-5 opponents alone?",
				"Two teams of two vie for dominance",
				"Two teams of three face off on the battlefield",
				"Two teams of four head to battle",
				"Three teams of two, can you handle it?",
				"Can your team defeat 1-2 others?",
				"Tournament Game"
			};

			Utils::Stream st;
			st << (byte)0;
			byte c = 0;
			for(byte z = 0; z < 3; z ++)
			{
				for(uint i = mi_min[z]; i < _mapindex.size() && i < mi_min[z + 1]; i ++)
				{
					if(_mapindex[i].size() == 0)
						continue;
					st << z << mapinfo_prefix[i][0] << (map_desc[i].length() == 0 ? defaultDesc[i] : map_desc[i]);
					st << (map_longDesc[i].length() == 0 ? defaultDescLong[i] : map_longDesc[i]);
					c ++;
				}
			}
			st[0] = c;
			return st;
		}

		std::vector<byte> LadderData::GetLadderData()
		{
			const char* desc[] = {
				"Solo Games",
				"Team Games",
				"Free For all Games",
				"2 Player Team",
				"3 Player Team",
				"4 Player Team",
				"Solo Games",
				"2 Player Team",
				"3 Player Team",
				"4 Player Team",
			};
			Utils::Stream st;
			byte c = 10;
			st << c;
			for(byte i = 0; i < c; i ++)
			{
				st << ladder_shortname[i][3] << ladder_shortname[i][2] << ladder_shortname[i][1] << ladder_shortname[i][0];
				st << (ladder_desc[i].length() > 0 ? ladder_desc[i] : desc[i]);
				st << ladder_url[i];
			};
			return st;
		}

		void LadderData::__AddMap( std::string mapname, byte type )
		{
			if(_maplist.size() >= 100)
				return;
			size_t i = 0;
			for(; i < _maplist.size(); i ++)
			{
				if(_STR.strcmpi(_maplist[i].c_str(), mapname.c_str()) == 0)
					break;
			}
			if(i == _maplist.size())
			{
				_maplist.push_back(mapname);
			}
			if(type >= (byte)_mapindex.size())
			{
				_mapindex.resize(type + 1);
			}
			_mapindex[type].push_back(i);
		}
	}
}
