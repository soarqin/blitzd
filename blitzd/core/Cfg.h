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

		DECL_CFG(uint, TcpPort);
		DECL_CFG(uint, UdpPort);
		DECL_CFG(std::string, Name);
		DECL_CFG(std::string, DBEngine);
		DECL_CFG(std::string, DBFile);
		DECL_CFG(uint, HashtableSize);

		DECL_CFG(std::string, CfgDir);
		DECL_CFG(std::string, FilesDir);
		DECL_CFG(std::string, GamesDir);
		DECL_CFG(std::string, DataDir);

		DECL_CFG(std::string, NewsFile);
		DECL_CFG(std::string, MotdFile);
		DECL_CFG(std::string, CheckRevFile);
		DECL_CFG(std::string, ChannelFile);
		DECL_CFG(std::string, LadderMapFile);
	};
	extern Cfg cfg;
}

#endif // _CFG_H_

