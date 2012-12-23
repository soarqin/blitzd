#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "network/TcpClient.h"
#include "utils/Stream.h"
#include "cache/UserCache.h"
#include "cache/ChannelCache.h"
#include "cache/GameCache.h"
#include "cache/CacheFunc.h"
#include "BNetDefs.h"

namespace Core
{
	class Client:
		public Network::TcpClient
	{
	public:
		enum Stage_t
		{
			CONNECTED,
			BNET,
			BNET_LOGGEDIN,
			FILETRANS,
			FILERAW,
			BAAL,
		};
	public:
		Client(int);
		virtual ~Client();
	protected:
		virtual void OnRecv(byte *, size_t);

	public:
		void SetAuthInfo(uint platform, uint gameid, uint ver, uint gamelang, uint natip, uint timezone, uint locale, uint language, std::string& countryab, std::string& country);
		inline Stage_t GetStage() {return _stage;}
		void SetStage(Stage_t s);
		inline uint GetToken() {return _token;}
		inline uint GetSession() {return _session;}
		inline uint GetProtocolVer() {return _protocolVer;}
		inline void SetGameID(uint gameid) {_gameid = gameid;}
		inline uint GetGameID() {return _gameid;}
		inline uint GetVersion() {return _version;}
		void SetUser(Cache::UserCacheItem::Pointer u);
		inline Cache::UserCacheItem::Pointer GetUser() {return _user;}
		inline std::string GetUsername() {return (_user.get() == NULL) ? "" : _user->GetUsername();}
		void SetGame(Cache::GameCacheItem * g);
		inline Cache::GameCacheItem * GetGame() {return _game;}
		void SetChannel(Cache::ChannelCacheItem * ch);
		inline Cache::ChannelCacheItem * GetChannel() {return _channel;}
		inline void SetProof(const byte * s, const byte * c) {memcpy(_server_proof, s, 20); memcpy(_client_proof, c, 20);}
		inline const byte * GetServerProof() {return _server_proof;}
		inline const byte * GetClientProof() {return _client_proof;}
		inline uint GetGameIP() {return _gameIp;}
		inline ushort GetGamePort() {return _gamePort;}
		inline void SetGameIP(uint ip) {_gameIp = ip;}
		inline void SetGamePort(ushort port) {_gamePort = port;}
		inline const char * GetStats() {return _stats.c_str();}
		inline uint GetLatency() { return _latency; }
		inline const char * GetCountryABR() {return _countryab.c_str();}
		inline uint GetFlag() {return _flag | ((_user.get() == NULL) ? 0 : _user->GetFlag());}
		inline void SetFlag(uint f) {_flag = f;}
		inline void AddFlag(uint f) {_flag |= f;}
		inline void DelFlag(uint f) {_flag &= ~f;}
		inline std::string GetDND() {return _dnd;}
		inline std::string GetAway() {return _away;}
		inline void SetDND(std::string d) {_dnd = d;}
		inline void SetAway(std::string a) {_away = a;}
		inline void SetLastWhisper(std::string w) {_lastWhisper = w;}
		inline std::string GetLastWhisper() {return _lastWhisper;}
		void CheckStart(uint);
		void CheckEnd(uint);
		void SendPing();
	private:
		void ProcessBNet();
		void ProcessBNetLoggedIn();
		void ProcessFile();
		void ProcessBaal();
		void BuildStats();

	private:
		Utils::Stream _buffer;
		Stage_t _stage;
		uint _platform, _gameid, _version, _gamelang, _natip, _tzone, _locale, _language;
		uint _gameIp;
		ushort _gamePort;
		uint _check_sid;
		uint _checkTick;
		uint _latency;
		byte _protocolVer;
		uint _session, _token;
		uint _flag;
		std::string _countryab, _country;
		Cache::UserCacheItem::Pointer _user;
		Cache::GameCacheItem * _game;
		Cache::ChannelCacheItem * _channel;
		std::string _stats;
		byte _server_proof[20], _client_proof[20];
		std::string _dnd, _away;
		std::string _lastWhisper;
	};

	class ClientPool
	{
	public:
		ClientPool();
		void Add(Client *);
		void AddByToken(Client *);
		void Remove(std::string);
		void Remove(uint);
		Client * operator[](std::string);
		Client * operator[](uint);
		inline uint GetCount() {return (uint)_clientMap.size();}
	private:
		typedef std::unordered_map<std::string, Client *, Cache::LowerCaseHash, Cache::CompareInCaseEqual> ClientMap_t;
		typedef std::unordered_map<uint, Client *> TokenMap_t;
		ClientMap_t _clientMap;
		TokenMap_t _tokenMap;
	};

	extern ClientPool clientPool;
}

#endif // _CLIENT_H_
