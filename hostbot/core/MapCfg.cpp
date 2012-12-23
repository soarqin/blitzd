#include "Config.h"
#include "MapCfg.h"
#include "BotCfg.h"
#include "SavedGame.h"
#include "utils/File.h"
#include "utils/Log.h"
#include "utils/Stream.h"
#include "crypto/Crc32.h"
#ifdef _MSC_VER
#pragma comment(lib, "../storm/StormLib.lib")
#endif

namespace HostBot
{
	inline uint rol(uint v, uint n)
	{
		return (v >> (32-n)) | (v << n);
	}

	MapCfg::MapCfg( const char * alias, const char * filePath, const char * fakePath, uint minStartCount, bool defRef )
	{
		_loadedChecksum = 0;
		_savedGame = false;
		_minStartCount = minStartCount;
		_path = filePath;
		Load();
		_alias = alias;
		_fakepath = fakePath;
		_defaultReferee = defRef;
	}

	MapCfg::MapCfg( const char * savedFilename )
	{
		_savedGame = true;
		SavedGame sg;
		sg.Load(savedFilename);
		MapCfg::Pointer ptr = MapCfgPool::GetSingleton().FindByPath(sg.GetMapName());
		if(ptr.get() == NULL)
		{
			throw;
		}
		_path = ptr->_path;
		_fakepath = sg.GetMapName();

		_mapCRC = ptr->_mapCRC;
		_mapHash = ptr->_mapHash;
		_mapSize = ptr->_mapSize;
		memcpy(_shaHash, ptr->_shaHash, 20);
		_rawWidth = ptr->_rawWidth;
		_rawHeight = ptr->_rawHeight;

		_loadedChecksum = sg.GetStageChecksum();
		_slots = sg.GetSlots();
		_stats = sg.GetStats();
		_selectMode = sg.GetSelectMode();

		_numPlayers = 0;
		_rawNumPlayers = 0;
		size_t c = _slots.size();
		for(size_t i = 0; i < c; ++ i)
		{
			if(_slots[i].GetTeam() < 12)
				++ _numPlayers;
			if(_slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OCCUPIED)
				++ _rawNumPlayers;
		}
		_minStartCount = _rawNumPlayers;
	}

	void MapCfg::Load()
	{
		HANDLE mpq;
		Utils::FileStream fs;
		if(!fs.Open(_path.c_str()))
			throw;
		_mapSize = (uint)fs.Size();
		_data.resize(_mapSize);
		fs.Read(&_data[0], _mapSize);
		fs.Close();
		Crypto::CRC32 crc32;
		_mapCRC = crc32.CalcData(&_data[0], _mapSize);
		_crclist.clear();
		for(size_t pos = 0; pos < _mapSize; pos += 1442)
		{
			size_t sz = 1442;
			if(pos + 1442 > _mapSize)
				sz = _mapSize - pos;
			_crclist.push_back(crc32.CalcData(&_data[pos], sz));
		}
		LOG_DEBUG(("%d %08X", _mapSize, _mapCRC));
		if(!SFileOpenArchive(_path.c_str(), 0, 0, &mpq))
			throw;
		uint h;
		Crypto::SHA1Context sha1;
		Crypto::SHA1Reset(&sha1);
		if(!script_hash(mpq, &h, &sha1))
		{
			SFileCloseArchive(mpq);
			throw;
		}
		static const char * hash_fn[] =
		{
			"war3map.w3e",
			"war3map.wpm",
			"war3map.doo",
			"war3map.w3u",
			"war3map.w3b",
			"war3map.w3d",
			"war3map.w3a",
			"war3map.w3q"
		};
		for(int i = 0; i < 8; i ++)
		{
			uint hh;
			if(do_hash(mpq, hash_fn[i], &hh, &sha1))
			{
				h = rol(h ^ hh, 3);
			}
		}
		_mapHash = h;
		Crypto::SHA1Result(&sha1, _shaHash);
		LOG_DEBUG(("%08X", _mapHash));
		LOG_DEBUG((_shaHash, 20));

		read_info(mpq);
		SFileCloseArchive(mpq);
	}

	uint MapCfg::do_hash(std::vector<byte>& buf, Crypto::SHA1Context * context)
	{
		byte * buf_ = &buf[0];
		uint size = (uint)buf.size();
		Crypto::SHA1Input(context, buf_, size);
		uint z = size >> 2;
		uint r = 0;
		for(uint i = 0; i < z; ++ i)
		{
			r ^= *(unsigned int *)buf_;
			r = rol(r, 3);
			buf_ += 4;
		}
		uint pad = size & 3;
		for(uint i = 0; i < pad; ++ i)
		{
			r ^= (unsigned int)*buf_;
			r = rol(r, 3);
			buf_ ++;
		}
		return r;
	}
	bool MapCfg::do_hash(HANDLE hmpq, const char * filename, uint * hash, Crypto::SHA1Context * context, bool fromFile)
	{
		*hash = 0;
		HANDLE hf;
		std::vector<byte> buf;
		if(fromFile)
		{
			Utils::FileStream fs;
			LOG_DEBUG(("Reading %s", (botCfg.ScriptPath() + filename).c_str()));
			if(!fs.Open(botCfg.ScriptPath() + filename))
				return false;
			uint size = (uint)fs.Size();
			buf.resize(size);
			fs.Read(&buf[0], size);
			LOG_DEBUG(("%d", size));
			fs.Close();
		}
		else
		{
			if(!SFileOpenFileEx(hmpq, filename, 0, &hf))
				return false;
			uint size = SFileGetFileSize(hf, NULL);
			if(size == 0)
			{
				return true;
			}
			buf.resize(size);
			DWORD rd;
			SFileReadFile(hf, &buf[0], size, &rd, NULL);
			SFileCloseFile(hf);
		}
		*hash = do_hash(buf, context);
		return true;
	}
	bool MapCfg::script_hash(HANDLE hmpq, uint * hash, Crypto::SHA1Context * context)
	{
		*hash = 0;
		uint h1 = 0, h2 = 0, h3 = 0;
		if(!do_hash(hmpq, "scripts\\common.j", &h1, context, false) && !do_hash(hmpq, "common.j", &h1, context, true))
			return false;
		if(!do_hash(hmpq, "scripts\\blizzard.j", &h2, context, false) && !do_hash(hmpq, "blizzard.j", &h2, context, true))
			return false;
		SHA1Input(context, (unsigned char *)"\x9E\x37\xF1\x03", 4);
		if(!do_hash(hmpq, "war3map.j", &h3, context) && !do_hash(hmpq, "scripts\\war3map.j", &h3, context))
			return false;
		*hash = rol(rol(rol(h1 ^ h2, 3) ^ 0x3F1379E, 3) ^ h3, 3);
		return true;
	}

	bool MapCfg::read_info( HANDLE hmpq )
	{
		HANDLE hf;
		if( !SFileOpenFileEx( hmpq, "war3map.w3i", 0, &hf ) )
			return false;
		uint size = SFileGetFileSize( hf, NULL );

		if( size == 0 || size == (uint)-1 )
		{
			SFileCloseFile(hf);
			return false;
		}

		std::vector<byte> buf;
		buf.resize(size);
		DWORD rd;
		if( !SFileReadFile( hf, &buf[0], size, &rd, NULL ) )
		{
			SFileCloseFile(hf);
			return false;
		}

		Utils::Stream st(buf);
		uint format;
		st>>format;

		if( format == 18 || format == 25 )
		{
			st += 4;					// number of saves
			st += 4;					// editor version
			std::string garbage;
			st>>garbage;		// map name
			st>>garbage;		// map author
			st>>garbage;		// map description
			st>>garbage;		// players recommended

			st += 32;				// camera bounds
			st += 16;				// camera bounds complements
			st >> _rawWidth >> _rawHeight >> _rawFlags; // map width, height, flags
			st += 1;					// map main ground type

			st += 4;				// campaign background number
			if( format == 25 )
				st>>garbage;	// path of custom loading screen model

			st>>garbage;		// map loading screen text
			st>>garbage;		// map loading screen title
			st>>garbage;		// map loading screen subtitle

			if( format == 18 )
				st += 4;				// map loading screen number
			else
			{
				st += 4;				// used game data set
				st>>garbage;	// prologue screen path
			}

			st>>garbage;		// prologue screen text
			st>>garbage;		// prologue screen title
			st>>garbage;		// prologue screen subtitle

			if( format == 25 )
			{
				st += 4;				// uses terrain fog
				st += 4;				// fog start z height
				st += 4;				// fog end z height
				st += 4;				// fog density
				st += 1;				// fog red value
				st += 1;				// fog green value
				st += 1;				// fog blue value
				st += 1;				// fog alpha value
				st += 4;				// global weather id
				st>>garbage;	// custom sound environment
				st += 1;				// tileset id of the used custom light environment
				st += 1;				// custom water tinting red value
				st += 1;				// custom water tinting green value
				st += 1;				// custom water tinting blue value
				st += 1;				// custom water tinting alpha value
			}

			st>>_rawNumPlayers;		// number of players

			_numPlayers = 0;
			for( uint i = 0; i < _rawNumPlayers; i++ )
			{
				GameSlot Slot( 0, 255, GameSlot::SLOTSTATUS_OPEN, 0, 0, 1, GameSlot::SLOTRACE_RANDOM );
				uint Color;
				uint Status;
				uint Race;

				st>>Color;			// Color
				Slot.SetColor( Color );
				st>>Status;			// status

				if( Status == 1 )
				{
					Slot.SetSlotStatus( GameSlot::SLOTSTATUS_OPEN );
					++ _numPlayers;
				}
				else if( Status == 2 )
				{
					Slot.SetSlotStatus( GameSlot::SLOTSTATUS_OCCUPIED );
					Slot.SetComputer( 1 );
					Slot.SetComputerType( GameSlot::SLOTCOMP_NORMAL );
					++ _numPlayers;
				}
				else
				{
					Slot.SetSlotStatus( GameSlot::SLOTSTATUS_CLOSED );
				}

				st>>Race;			// race

				if( Race == 1 )
					Slot.SetRace( GameSlot::SLOTRACE_HUMAN );
				else if( Race == 2 )
					Slot.SetRace( GameSlot::SLOTRACE_ORC );
				else if( Race == 3 )
					Slot.SetRace( GameSlot::SLOTRACE_UNDEAD );
				else if( Race == 4 )
					Slot.SetRace( GameSlot::SLOTRACE_NIGHTELF );
				else
					Slot.SetRace( GameSlot::SLOTRACE_RANDOM );

				st += 4;				// fixed start position
				st>>garbage;	// player name
				st += 4;				// start position x
				st += 4;				// start position y
				st += 4;				// ally low priorities
				st += 4;				// ally high priorities

				if( Slot.GetSlotStatus( ) != GameSlot::SLOTSTATUS_CLOSED )
					_slots.push_back( Slot );
			}

			st>>_rawNumTeams;		// number of teams

			for( uint i = 0; i < _rawNumTeams; i++ )
			{
				uint Flags;
				uint PlayerMask;

				st>>Flags;			// flags
				st>>PlayerMask;		// player mask

				for( unsigned char j = 0; j < 12; j++ )
				{
					if( PlayerMask & 1 )
					{
						for( std::vector<GameSlot>::iterator k = _slots.begin( ); k != _slots.end( ); ++ k )
						{
							if( (*k).GetColor( ) == j )
								(*k).SetTeam( i );
						}
					}

					PlayerMask >>= 1;
				}

				st>>garbage;	// team name
			}

			LOG_DEBUG(( "map width = %u", _rawWidth ));
			LOG_DEBUG(( "map height = %u", _rawHeight ));
			LOG_DEBUG(( " map num of players = %u/%u", _numPlayers, _rawNumPlayers ));
			_numTeams = _rawNumTeams;
			LOG_DEBUG(( " map num of teams = %u", _numTeams ));

			uint SlotNum = 1;

			for( std::vector<GameSlot>::iterator i = _slots.begin( ); i != _slots.end( ); ++ i )
			{
				LOG_DEBUG(( "map slot %d =", SlotNum ));
				std::vector<byte> r = (*i).GetArray( );
				LOG_DEBUG((&r[0], (int)r.size()));
				SlotNum++;
			}

			if( _rawFlags & 4 )
			{
				LOG_DEBUG(( "[MAP] found melee map, initializing slots" ));

				// give each slot a different team and set the race to random

				byte Team = 0;

				for( std::vector<GameSlot>::iterator i = _slots.begin( ); i != _slots.end( ); ++ i )
				{
					(*i).SetTeam( Team ++ );
					(*i).SetRace( GameSlot::SLOTRACE_RANDOM );
				}
			}
		}


		SFileCloseFile(hf);
		return true;
	}

	void MapCfgPool::Add( MapCfg::Pointer cfg )
	{
		_cfgList.push_back(cfg);
	}

	size_t MapCfgPool::GetCount()
	{
		return _cfgList.size();
	}

	MapCfg::Pointer MapCfgPool::operator[]( size_t idx )
	{
		if(idx < _cfgList.size())
			return _cfgList[idx];
		return MapCfg::Pointer();
	}

	MapCfg::Pointer MapCfgPool::operator[]( std::string name )
	{
		size_t c = _cfgList.size();
		for(size_t i = 0; i < c; ++ i)
		{
			if(_STR.strcmpi(name.c_str(), _cfgList[i]->GetAlias().c_str()) == 0)
				return _cfgList[i];
		}
		return (*this)[atoi(name.c_str()) - 1];
	}

	MapCfg::Pointer MapCfgPool::FindByPath( std::string name )
	{
		size_t c = _cfgList.size();
		for(size_t i = 0; i < c; ++ i)
		{
			if(_STR.strcmpi(name.c_str(), _cfgList[i]->GetFakePath().c_str()) == 0 || _STR.strcmpi(name.c_str(), _cfgList[i]->GetPath().c_str()) == 0)
				return _cfgList[i];
		}
		return MapCfg::Pointer();
	}
}
