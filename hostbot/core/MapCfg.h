#ifndef _MAPCFG_H_
#define _MAPCFG_H_

#include "GameSlot.h"
#include "utils/Singleton.h"
#include "utils/SharedPtr.h"
#include "crypto/SHA1Hash.h"
#if defined(WIN32) && !defined(_MSC_VER)
extern "C"
{
#endif
#include "storm/StormLib.h"
#if defined(WIN32) && !defined(_MSC_VER)
}
#endif

namespace HostBot
{
	class MapCfg:
		public Utils::SharedClass<MapCfg>
	{
	public:
		MapCfg(const char * alias, const char * filePath, const char * fakePath, uint minStartCount, bool defRef);
		MapCfg(const char * savedFilename);

		inline std::string GetAlias() {return _alias;}
		inline std::string GetPath() {return _path;}
		inline std::string GetFakePath() {return _fakepath;}
		inline uint GetSize() { return _mapSize; }
		inline uint GetCRC() { return _mapCRC; }
		inline uint GetHash() { return _mapHash; }
		inline uint GetWidth() { return _rawWidth; }
		inline uint GetHeight() { return _rawHeight; }
		inline uint GetFlags() { return _rawFlags; }
		inline uint GetRawNumPlayers() { return _rawNumPlayers; }
		inline uint GetNumPlayers() { return _numPlayers; }
		inline uint GetNumTeams() { return _numTeams; }
		inline byte* GetSHAHash() {return _shaHash;}
		inline std::vector<GameSlot>& GetSlots() { return _slots; }
		inline bool GetDefaultReferee() {return _defaultReferee;}
		inline uint GetMinStartCount() {return _minStartCount;}
		inline bool GetSavedGame() {return _savedGame;}
		inline uint GetLoadedChecksum() {return _loadedChecksum;}
		inline std::string GetStats() {return _stats;}
		inline byte GetSelectMode() {return _selectMode;}
		inline byte * GetData() {return &_data[0];}
		inline std::vector<uint>& GetCrcList() {return _crclist;}
	protected:
		std::string _alias;
		std::string _path;
		std::string _fakepath;
		std::vector<byte> _data;
		std::vector<uint> _crclist;
		byte _shaHash[20];
		uint _mapSize, _mapCRC, _mapHash;
		uint _rawWidth, _rawHeight, _rawFlags, _rawNumPlayers, _rawNumTeams, _numPlayers, _numTeams;
		uint _minStartCount;
		std::vector<GameSlot> _slots;
		bool _defaultReferee;
		bool _savedGame;
		uint _loadedChecksum;
		std::string _stats;
		byte _selectMode;

		void Load();
	private:
		uint do_hash(std::vector<byte>& buf, Crypto::SHA1Context * context);
		bool do_hash(HANDLE hmpq, const char * filename, uint * hash, Crypto::SHA1Context * context, bool fromFile = false);
		bool script_hash(HANDLE hmpq, uint * hash, Crypto::SHA1Context * context);
		bool read_info(HANDLE hmpq);
	};

	class MapCfgPool:
		public Utils::Singleton<MapCfgPool>
	{
	public:
		void Add(MapCfg::Pointer);
		size_t GetCount();
		MapCfg::Pointer operator[] (size_t);
		MapCfg::Pointer operator[] (std::string);
		MapCfg::Pointer FindByPath(std::string);
	private:
		std::vector<MapCfg::Pointer> _cfgList;
	};
}

#endif // _MAPCFG_H_
