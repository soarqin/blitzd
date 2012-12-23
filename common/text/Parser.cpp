#include "Config.h"
#include "Parser.h"

namespace Text
{
	void SeperateString( std::vector<std::string>& result, std::string str, char delimiter, size_t off )
	{
		while( off < str.length() && str[off] == delimiter )
			++ off;
		size_t pos;
		while((pos = str.find(delimiter, off)) != std::string::npos)
		{
			result.push_back(std::string(str.begin() + off, str.begin() + pos));
			off = pos + 1;
			while( off < str.length() && str[off] == delimiter )
				++ off;
		}
		if(off < str.length())
			result.push_back(std::string(str.begin() + off, str.end()));

	}
}
