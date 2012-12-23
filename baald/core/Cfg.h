#ifndef _CFG_H_
#define _CFG_H_

namespace Core
{
#define DECL_CFG(t,n) \
private: \
	t _##n; \
public: \
	inline t Get##n() {return _##n;} \

	class Cfg
	{
	public:
		void Load(const char * filename);

		DECL_CFG(std::string, BlitzDAddr);
		DECL_CFG(uint, BlitzDPort);
		DECL_CFG(uint, TcpPort);
		DECL_CFG(std::string, Name);
		DECL_CFG(std::string, Description);
		DECL_CFG(uint, HashtableSize);

		DECL_CFG(std::string, CfgDir);
	};
	extern Cfg cfg;
}

#endif // _CFG_H_

