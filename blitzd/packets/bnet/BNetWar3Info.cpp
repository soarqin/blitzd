#include "Config.h"
#include "BNetWar3Info.h"
#include "games/war3/LadderInfo.h"

namespace Packets
{
	namespace BNet
	{
		bool BNetWar3Info::Pack()
		{
			_packet << subcmd;
			switch(subcmd)
			{
			case 2:
				{
					_packet << cookie << (byte)1 << tag;
					switch(tag)
					{
					case 0x55524c:
						_packet << 0xBF1F1047;
						_packet << Games::War3::LadderInfo::GetSingleton().GetUrlData();
						break;
					case 0x4d4150:
						_packet << 0x70E2E0D5;
						_packet << Games::War3::LadderInfo::GetSingleton().GetMapName();
						break;
					case 0x54595045:
						_packet << 0x7C87DEEE;
						_packet << Games::War3::LadderInfo::GetSingleton().GetMapInfo();
						break;
					case 0x44455343:
						_packet << 0xA4F0A22F;
						_packet << Games::War3::LadderInfo::GetSingleton().GetMapDesc();
						break;
					case 0x4c414452:
						_packet << 0x3BADE25A;
						_packet << Games::War3::LadderInfo::GetSingleton().GetLadderData();
						break;
					default:
						return false;
					}
					_packet << (byte)(is_last ? 0 : 1);
				}
				return true;
			default:
				return false;
			}
			return false;
		}
	}
}
