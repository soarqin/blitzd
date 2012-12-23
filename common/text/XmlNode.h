#ifndef _XMLCONFIG_H_
#define _XMLCONFIG_H_

namespace Text
{
	class XmlNode
	{
	public:
		XmlNode(const char *);
		~XmlNode();
		XmlNode * GetChild(const char *);
		XmlNode * GetNext(const char *);
		const char * GetAttrString(const char *, const char *);
		int GetAttrInteger(const char *, int);
		double GetAttrReal(const char *, double);
		void ReadArray(std::vector<std::vector<std::string> >& result, std::vector<std::string>& keys, const char *);
	private:
		void * _data;
		XmlNode * _parent;

	private:
		XmlNode();
	};
}

#endif // _XMLCONFIG_H_
