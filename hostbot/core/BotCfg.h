#ifndef _BOTCFG_H_
#define _BOTCFG_H_

namespace HostBot
{
	class BotCfg
	{
	public:
		void Load(const char * filename);

		inline std::string Password() const { return _password; }

		inline bool SaveReplay() const { return _saveReplay; }
		inline uint SyncLimit() const { return _syncLimit; }
		inline uint GameVersion() const { return _gameVersion; }

		inline bool AutoGame() const { return _autoGame; }
		inline std::string AutoName() const { return _autoName; }
		inline uint AutoMaxCount() const { return _autoMaxCount; }
		inline std::string AutoDefMap() const { return _autoDefMap; }

		inline uint TcpPort() const { return _tcpPort; }
		inline uint UdpPort() const { return _udpPort; }

		inline std::string ScriptPath() const { return _scriptPath; }
	protected:
		std::string _password;

		bool _saveReplay;
		uint _syncLimit;
		uint _gameVersion;

		bool _autoGame;
		std::string _autoName;
		uint _autoMaxCount;
		std::string _autoDefMap;

		uint _tcpPort, _udpPort;

		std::string _scriptPath;
	};
	extern BotCfg botCfg;
}

#endif // _BOTCFG_H_
