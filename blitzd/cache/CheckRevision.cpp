#include "Config.h"
#include "CheckRevision.h"
#include "core/Cfg.h"
#include "utils/Memory.h"
#include "utils/Random.h"
#include "utils/File.h"
#include "utils/DataConv.h"
#include "text/Parser.h"
#include "text/XmlNode.h"

namespace Cache
{
	void CheckRevision::Load()
	{
		uint a = Utils::GetRandom32() & 0x7FFFFFFF;
		uint b = Utils::GetRandom32() & 0x7FFFFFFF;
		uint c = Utils::GetRandom32() & 0x7FFFFFFF;
		byte rop1 = Utils::GetRandom8() % 3;
		byte rop2 = Utils::GetRandom8() % 3;
		byte rop3 = Utils::GetRandom8() % 3;
		byte rop4 = Utils::GetRandom8() % 3;
		const char rop[] = {'+', '-', '^'};
		char __formula[1024];
		sprintf(__formula, "A=%u B=%u C=%u 4 A=A%cS B=B%cC C=C%cA A=A%cB", a, b, c, rop[rop1], rop[rop2], rop[rop3], rop[rop4]);
		_formula = __formula;
		_formula = "A=418025954 B=402661753 C=1258758744 4 A=A+S B=B^C C=C-A A=A-B";
		LOG_DEBUG(("Generated formula: %s", __formula));

		Text::XmlNode cfg_((Core::cfg.GetCfgDir() + "/" + Core::cfg.GetCheckRevFile()).c_str());
		std::auto_ptr<Text::XmlNode> node(cfg_.GetChild("Revisions"));
		if(node.get() != NULL)
		{
			std::vector<std::string> keys;
			keys.push_back("gameName");
			keys.push_back("gameId");
			keys.push_back("file1");
			keys.push_back("file2");
			keys.push_back("file3");
			keys.push_back("exever");
			std::vector<std::vector<std::string> > results;
			node->ReadArray(results, keys, "Revision");
			size_t cnt = results.size();
			for(size_t i = 0; i < cnt; ++ i)
			{
				Revision rev;
				rev.gameName = results[i][0];
				rev.gameId = Utils::Reverse(*(uint*)results[i][1].c_str());
				std::string fn[3];
				fn[0] = results[i][2];
				fn[1] = results[i][3];
				fn[2] = results[i][4];
				if(_STR.strcmpni(results[i][5].c_str(), "0x", 2) == 0)
					rev.exeVer = strtoul(results[i][5].c_str() + 2, NULL, 16);
				else
					rev.exeVer = strtoul(results[i][5].c_str(), NULL, 10);
				if(fn[0].length() > 0 && fn[1].length() > 0 && fn[2].length() > 0 && rev.gameId != 0)
				{
					fn[0] = results[i][1] + '/' + fn[0];
					fn[1] = results[i][1] + '/' + fn[1];
					fn[2] = results[i][1] + '/' + fn[2];
					if(!Calculate(_formula, fn, rev.chksum))
					{
						LOG_ERROR(("Failed to calculate checksum for %s", rev.gameName.c_str()));
						continue;
					}
					for(int i = 0; i < 8 ; i ++)
					{
						LOG_DEBUG(("Checksum[%d] = 0x%08X", i, rev.chksum[i]));
					}
					_revlist.push_back(rev);
				}
			}
		}
	}

	CheckRevision::Revision* CheckRevision::findRev( int verIndex, uint chksum, uint exeVer )
	{
		for(size_t i = 0; i < _revlist.size(); i ++)
		{
			if(_revlist[i].chksum[verIndex] == chksum && (_revlist[i].exeVer == 0 || _revlist[i].exeVer == exeVer))
				return &_revlist[i];
		}
		return NULL;
	}

	const char * CheckRevision::GetFormula()
	{
		return _formula.c_str();
	}

	bool CheckRevision::Calculate( std::string& formulaStr, std::string * fileNames, uint * result, uint sindex /*= 0*/, uint eindex /*= 7*/ )
	{
		const uint mpqHashValues[] =
		{
			0xE7F4CB62,
			0xF6A14FFC,
			0xAA5504AF,
			0x871FCDC2,
			0x11BF6A18,
			0xC57292E6,
			0x7927D27E,
			0x2FEC8733
		};
		uint oval[4], val[4], opTarg[4], opVal1[4], opVal2[4];
		char opType[4];

		std::vector<std::string> tokens;
		Text::SeperateString(tokens, formulaStr, ' ');

		int formulaIdx = 0;
		size_t i;

		for (i = 0; i < tokens.size(); ++i)
		{
			std::string token = tokens[i];
			
			if (token.find('=') != std::string::npos)
			{
				std::vector<std::string> nameTokens;
				Text::SeperateString(nameTokens, token, '=');
				if (nameTokens.size() != 2)
				{
					if(token.length() > 1)
						return false;
					continue;
				}

				int targ = ((nameTokens[0][0] == 'S') ? 3 : (nameTokens[0][0] - 'A'));

				std::string v = nameTokens[1];

				if (v[0] >= '0' && v[0] <= '9')
				{
					oval[targ] = strtoul(v.c_str(), NULL, 10);
				}
				else
				{
					opTarg[formulaIdx] = targ;

					opVal1[formulaIdx] = (v[0] == 'S') ? 3 : (v[0] - 'A');
					opType[formulaIdx] = v[1];
					opVal2[formulaIdx] = (v[2] == 'S') ? 3 : (v[2] - 'A');

					formulaIdx++;
				}
			}
		}

		std::vector<byte> buf[3];
		for (i = 0; i < 3; ++ i)
		{
			Utils::FileStream fs;
			fs.Open(Core::cfg.GetGamesDir() + '/' + fileNames[i]);
			uint fLen = (uint)fs.Size();
			uint bufSize = (fLen + 1023) & (~1023);
			buf[i].resize(bufSize);
			fs.Read(&buf[i][0], fLen);
			fs.Close();

			byte pad = 0xff;
			for (uint j = fLen; j < bufSize; ++j)
			{
				buf[i][j] = pad--;
			}
		}

		for(uint verIndex = sindex; verIndex <= eindex; verIndex ++)
		{
			memcpy(val, oval, sizeof(val));

			val[0] ^= mpqHashValues[verIndex];

			for (int i = 0; i < 3; i++)
			{
				uint* bp = (uint*)&buf[i][0];
				uint lc = (uint)buf[i].size() >> 2;

				for (uint j = 0; j < lc; ++ j)
				{
					val[3] = *(bp++);

					for (int k = 0; k < formulaIdx; k++)
					{
						switch (opType[k])
						{
						case '+':
							val[opTarg[k]] = val[opVal1[k]] + val[opVal2[k]];
							break;

						case '-':
							val[opTarg[k]] = val[opVal1[k]] - val[opVal2[k]];
							break;

						case '^':
							val[opTarg[k]] = val[opVal1[k]] ^ val[opVal2[k]];
							break;

						case '*':
							val[opTarg[k]] = val[opVal1[k]] * val[opVal2[k]];
							break;

						case '/':
							val[opTarg[k]] = val[opVal1[k]] / val[opVal2[k]];
							break;

						default:
							break;
						}
					}
				}
			}

			result[verIndex] = val[2];
		}
		return true;
	}
}
