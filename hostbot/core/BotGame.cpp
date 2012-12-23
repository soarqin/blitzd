#include "Config.h"
#include "BotGame.h"
#include "BotCfg.h"
#include "BotGameSet.h"
#include "BroadcastServer.h"
#include "utils/Stream.h"
#include "utils/SysTime.h"
#include "crypto/Crc32.h"
#include "text/Parser.h"

namespace HostBot
{
	static GameSlot RefereeSlot(0, 255, 0, 0, 12, 12, 96, 1, 100);
	void BotGame::TimerCheck(uint tick)
	{
		switch(_stage)
		{
		case STAGE_PREPARE:
			if(tick - _lastTrigTime >= 5120)
			{
				BroadcastSlotInfo();
				_lastTrigTime = tick;
			}
			if(_allReadyTime > 0 && tick - _allReadyTime >= 5120)
			{
				_allReadyTime = 0;
				SendStartGame();
			}
			break;
		case STAGE_COUNTDOWN:
			if(tick - _lastTrigTime >= 5500)
			{
				SendToAll(0x0BF7);
				_stage = STAGE_LOADING;
				_lastTrigTime = tick;
			}
			break;
		case STAGE_PLAYING:
			{
				uint ptime = tick - _lastTrigTime;
				if(ptime < 99)
					break;
				if(ptime > 250)
					ptime = 250;
				SendAllActions(ptime);
				_lastTrigTime = tick;
			}
		case STAGE_LAGGING:
			if(tick - _lastLagCheckTime >= 512)
			{
				CheckPlayersLag();
				_lastLagCheckTime = tick;
			}
			break;
        default:
			break;
		}
	}

	BotGame::BotGame(uint id, std::string& gameName, std::string& hostName, MapCfg::Pointer cfg, BotGame::MapSpeed_t spd, BotGame::MapVis_t vis, BotGame::MapObs_t obs, uint flag): _mapCfg(cfg)
	{
		_allReadyTime = 0;
		_lastLagCheckTime = _lastTrigTime = Utils::GetTicks();
		_id = id;
		_firstPID = 0;
		memset(_players, 0, sizeof(_players));
		_gameName = gameName;
		if(_gameName.length() > 31)
			_gameName.erase(_gameName.begin() + 31, _gameName.end());
		_hostName = hostName;
		if(_hostName.length() > 15)
			_hostName.erase(_hostName.begin() + 15, _hostName.end());
		_speed = spd;
		_vis = vis;
		_obs = obs;
		_flag = flag;
		_flag |= MAPFLAG_TEAMSTOGETHER | MAPFLAG_FIXEDTEAMS;
		_platform = 'W3XP';
		_version = botCfg.GameVersion();
		if((_mapCfg->GetFlags() & 0x20) == 0)
			_gameType = 9;
		else
			_gameType = 1;
		Utils::Stream st;
		st<<_platform<<_version<<_id<<Utils::GetTime()<<_gameName<<(byte)0;

		if(_mapCfg->GetSavedGame())
		{
			_randomSeed = _mapCfg->GetLoadedChecksum();
			st<<_mapCfg->GetStats();
		}
		else
		{
			Utils::Stream st2;
			st2<<BuildInfoFlag()<<(byte)0<<(ushort)_mapCfg->GetWidth()<<(ushort)_mapCfg->GetHeight()<<_mapCfg->GetHash();
			st2<<_mapCfg->GetFakePath()<<_hostName<<(byte)0;
			if(botCfg.GameVersion() >= 23)
				st2.append(_mapCfg->GetSHAHash(), 20);
			size_t len = st2.size();
			size_t blocks = len / 7;
			size_t pad = len % 7;
			byte data[7];
			byte flag_;
			for(size_t i = 0; i < blocks; ++ i)
			{
				flag_ = 1;
				for(size_t j = 0; j < 7; ++ j)
				{
					data[j] = st2[i * 7 + j];
					if((data[j] % 2) == 1)
					{
						flag_ |= (2 << j);
					}
					else
					{
						++ data[j];
					}
				}
				st<<flag_;
				st.append(data, 7);
			}
			if(pad > 0)
			{
				flag_ = 1;
				for(size_t j = 0; j < pad; ++ j)
				{
					data[j] = st2[blocks * 7 + j];
					if((data[j] % 2) == 1)
					{
						flag_ |= (2 << j);
					}
					else
					{
						++ data[j];
					}
				}
				st<<flag_;
				st.append(data, pad);
			}
			_randomSeed = Utils::GetTicks();
			_totalSlots = (uint)(_obs > MAPOBS_ONDEFEAT) ? 12 : _mapCfg->GetRawNumPlayers();
		}
		_slots.assign(_mapCfg->GetSlots().begin(), _mapCfg->GetSlots().end());
		if(_slots.size() < _totalSlots)
		{
			for(uint i = (uint)_slots.size(); i < _totalSlots; ++ i)
			{
				_slots.push_back(GameSlot(0, 255, 0, 0, 12, 12, 96, 1, 100));
			}
		}
		_usedSlots = 0;
		ReCalcUsedSlots();
		st<<(byte)0<<_totalSlots<<_gameType;
		_gameInfo = st;
		_startTime = Utils::GetTime();
		_stage = STAGE_PREPARE;
		Utils::Stream tosend;
		tosend<<_platform<<_version<<_id;
		BroadcastServer::GetSingleton().Broadcast(6112, 0x31F7, (byte *)tosend, tosend.size());
	}

	BotGame::~BotGame()
	{
		if(_stage == STAGE_PREPARE)
			DeCreate();
		_playersMutex.lock();
		for(uint i = 0; i < 12; ++ i)
		{
			if(_players[i])
			{
				_players[i]->LeaveGame();
				_players[i] = NULL;
			}
		}
		_playersMutex.unlock();
		_mapCfg.reset();
		_gameInfo.clear();
		_actionsMutex.lock();
		_actions.clear();
		_actionsMutex.unlock();
		_slotsMutex.lock();
		_slots.clear();
		_slotsMutex.unlock();
	}

	void BotGame::SendUDPInfo( uint ip, ushort port )
	{
		if(_stage != STAGE_PREPARE)
			return;
		Utils::Stream tosend(_gameInfo);
		tosend<<_usedSlots<<_totalSlots<<(uint)0<<(ushort)botCfg.TcpPort();
		BroadcastServer::GetSingleton().Send(ip, port, 0x30F7, tosend, tosend.size());
	}

	uint BotGame::BuildInfoFlag()
	{
		uint GameFlags = 0;

		// speed

		if( _speed == MAPSPEED_SLOW )
			GameFlags = 0x00000000;
		else if( _speed == MAPSPEED_NORMAL )
			GameFlags = 0x00000001;
		else
			GameFlags = 0x00000002;

		// visibility

		if( _vis == MAPVIS_HIDETERRAIN )
			GameFlags |= 0x00000100;
		else if( _vis == MAPVIS_EXPLORED )
			GameFlags |= 0x00000200;
		else if( _vis == MAPVIS_ALWAYSVISIBLE )
			GameFlags |= 0x00000400;
		else
			GameFlags |= 0x00000800;

		// observers

		if( _obs == MAPOBS_ONDEFEAT )
			GameFlags |= 0x00002000;
		else if( _obs == MAPOBS_ALLOWED )
			GameFlags |= 0x00003000;
		else if( _obs == MAPOBS_REFEREES )
			GameFlags |= 0x40000000;

		// teams/units/hero/race

		if( _flag & MAPFLAG_TEAMSTOGETHER )
			GameFlags |= 0x00004000;
		if( _flag & MAPFLAG_FIXEDTEAMS )
			GameFlags |= 0x00060000;
		if( _flag & MAPFLAG_UNITSHARE )
			GameFlags |= 0x01000000;
		if( _flag & MAPFLAG_RANDOMHERO )
			GameFlags |= 0x02000000;
		if( _flag & MAPFLAG_RANDOMRACES )
			GameFlags |= 0x04000000;

		return GameFlags;
	}

	bool BotGame::ApplyNewSlot( Player* ptr, byte newId )
	{
		if(_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN)
			return false;
		size_t i = 0;
		Utils::FastMutex::ScopedLock lock(_slotsMutex);
		if(_mapCfg->GetSavedGame())
		{
			i = newId;
			if(i >= _totalSlots || _slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OCCUPIED || _slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_CLOSED)
				return false;
		}
		else
		{
			for(i = 0; i < _totalSlots; ++ i)
			{
				if(_slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OPEN)
				{
					break;
				}
			}
			if(i == _totalSlots)
			{
				return false;
			}
		}

		Utils::FastMutex::ScopedLock lock2(_playersMutex);
		for(uint j = 0; j < 12; ++ j)
		{
			if(_players[j] == NULL)
			{
				_players[j] = ptr;
				ptr->SetSlotID(i + 1);
				ptr->SetPID(j + 1);
				if(j < _firstPID)
					_firstPID = (byte)j;
				_slots[i].SetPID(j + 1);
				_slots[i].SetDownloadStatus(255);
				_slots[i].SetSlotStatus(GameSlot::SLOTSTATUS_OCCUPIED);
				_slots[i].SetComputer(0);
				_slots[i].SetComputerType(GameSlot::SLOTCOMP_NORMAL);
				_slots[i].SetHandicap(100);
				if(_flag & MAPFLAG_RANDOMRACES)
					_slots[i].SetRace(GameSlot::SLOTRACE_RANDOM);
				if(!(_flag & MAPFLAG_FIXEDTEAMS))
				{
					if(j >= _mapCfg->GetNumPlayers())
					{
						_slots[i].SetTeam(12);
						_slots[i].SetColor(12);
					}
					else
					{
						byte colorused[12] = {0};
						for(size_t k = 0; k < _totalSlots; ++ k)
						{
							if(i != k && _slots[k].GetSlotStatus() == GameSlot::SLOTSTATUS_OCCUPIED && _slots[k].GetColor() < 12)
								colorused[_slots[k].GetColor()] = 1;
						}
						if(_slots[i].GetColor() >= 12 || colorused[_slots[i].GetColor()])
						{
							for(int k = 0; k < 12; ++ k)
							{
								if(!colorused[k])
								{
									_slots[i].SetColor(k);
									break;
								}
							}
						}
					}
				}
				ReCalcUsedSlots();
				return true;
			}
		}
		return false;
	}

	void BotGame::Kick( uint s, uint reason )
	{
		Utils::FastMutex::ScopedLock lock(_playersMutex);
		Utils::FastMutex::ScopedLock lock2(_slotsMutex);
		for(int j = 0; j < 12; ++ j)
		{
			if(_players[j] == NULL)
				continue;
			if(_players[j]->GetSession() == s)
			{
				if(_players[j]->GetLag() > 0)
				{
					SendStopLag(_players[j]);
					_players[j]->SetLag(0);
				}
				uint sid = _players[j]->GetSlotID();
				if(sid > 0)
				{
					if(sid > _mapCfg->GetRawNumPlayers())
						_slots[sid - 1] = RefereeSlot;
					else
						_slots[sid - 1] = _mapCfg->GetSlots()[sid - 1];
				}
				Utils::Stream st;
				st<<(byte)(1 + j);
				st<<reason;
				_players[j] = NULL;
				if(j == _firstPID)
				{
					_firstPID = 0;
					for(int i = 0; i < 12; ++ i)
					{
						if(_players[i] != NULL)
						{
							_firstPID = i;
							break;
						}
					}
				}
				SendToAll(0x07F7, st);
				ReCalcUsedSlots();
				SendSlotInfo();
				break;
			}
		}
		if(_stage == STAGE_PREPARE)
		{
			if(_allReadyTime > 0)
			{
				SendChatToAll("Due to a player left, the start timer is reset to 5 seconds, you can type !ready to cancel ready status to wait for more players");
				_allReadyTime = Utils::GetTicks();
			}
		}
		else
		{
			bool end = true;
			for(int j = 0; j < 12; ++ j)
			{
				if(_players[j] != NULL)
					end = false;
			}
			if(end)
			{
				BotGameSet::GetSingleton().PushToRemoveQueue(_id);
			}
		}
	}

	void BotGame::GetSlotInfo( Utils::Stream& st )
	{
		Utils::FastMutex::ScopedLock lock(_slotsMutex);
		st<<(ushort)(7 + 9 * _totalSlots)<<(byte)_totalSlots;
		for(size_t i = 0; i < _totalSlots; ++ i)
		{
			_slots[i].GetSlotData(st);
		}
		st<<_randomSeed;
		if(_mapCfg->GetSavedGame())
			st<<_mapCfg->GetSelectMode();
		else
			st<<(byte)(_gameType == 1 ? 3 : 0);
		st<<(byte)_mapCfg->GetNumPlayers();
	}

	void BotGame::SendPlayersInfo( Player& p )
	{
		if(_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN)
			return;
		bool built = false;
		Utils::Stream pst;
		Utils::FastMutex::ScopedLock lock(_playersMutex);
		for(byte i = 0; i < 12; ++ i)
		{
			if(_players[i] != NULL)
			{
				if(i + 1 != (byte)p.GetPID())
				{
					Utils::Stream st;
					_players[i]->BuildPlayerInfo(st, i + 1);
					p.Send(0x06F7, st);
				}
				else
				{
					if(!built)
					{
						p.BuildPlayerInfo(pst, p.GetPID());
						built = true;
					}
					SendToAll(0x06F7, pst, i);
				}
			}
		}
	}

	void BotGame::BuildMapCheck( Utils::Stream& st )
	{
		st<<(uint)1;
		st<<_mapCfg->GetPath()<<_mapCfg->GetSize()<<_mapCfg->GetCRC()<<_mapCfg->GetHash();
		if(botCfg.GameVersion() >= 23)
			st.append(_mapCfg->GetSHAHash(), 20);
	}

	void BotGame::SendSlotInfo()
	{
		if(_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN)
			return;
		Utils::Stream st;
		GetSlotInfo(st);
		Utils::FastMutex::ScopedLock lock(_playersMutex);
		for(uint i = 0; i < 12; ++ i)
		{
			if(_players[i] != NULL)
			{
				_players[i]->Send(0x09F7, st);
			}
		}
	}

	bool BotGame::SetMapProgress( uint id, uint prog )
	{
		if(_stage != STAGE_PREPARE)
			return true;
		uint sid;
		{
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			if(_players[id - 1] == NULL)
				return true;
			sid = _players[id - 1]->GetSlotID();
			if(sid == 0 || sid > _totalSlots)
				return true;
		}
		uint pg = prog * 100 / _mapCfg->GetSize();
		{
			Utils::FastMutex::ScopedLock lock(_slotsMutex);
			if(_slots[sid - 1].GetDownloadStatus() != pg)
			{
				_slots[sid - 1].SetDownloadStatus((byte)pg);
				SendSlotInfo();
			}
		}
		if(pg == 100)
		{
			SendWelcome(id);
			SendNumPlayerInfo();
			return true;
		}
		return false;
	}

	void BotGame::GetSlotStatus( uint& used, uint& total )
	{
		used = _usedSlots;
		total = _totalSlots;
	}

	void BotGame::SendToAll( ushort op, Utils::Stream& st, byte except )
	{
		std::vector<byte> sbuf;
		size_t len = st.size();
		sbuf.resize(4 + len);
		*(ushort *)&sbuf[0] = op;
		*(ushort *)&sbuf[2] = 4 + (ushort)len;
		if(len > 0)
			memcpy(&sbuf[4], (byte *)st, len);

		SendToAll(&sbuf[0], 4 + len, except);
	}

	void BotGame::SendToAll( ushort op, byte except )
	{
		byte sbuf[4];
		*(ushort *)&sbuf[0] = op;
		*(ushort *)&sbuf[2] = 4;
		SendToAll(&sbuf[0], 4, except);
	}

	void BotGame::SendToAll( const byte * buf, size_t len, byte except /*= 12*/ )
	{
		Utils::FastMutex::ScopedLock lock(_playersMutex);
		for(uint i = 0; i < 12; ++ i)
		{
			if(i != except && _players[i] != NULL)
			{
				((Network::TcpClient *)_players[i])->Send(buf, len);
			}
		}
	}

	void BotGame::SendTo( ushort op, Utils::Stream& st, std::vector<byte>& toid )
	{
		std::vector<byte> sbuf;
		size_t len = st.size();
		sbuf.resize(4 + len);
		*(ushort *)&sbuf[0] = op;
		*(ushort *)&sbuf[2] = 4 + (ushort)len;
		if(len > 0)
			memcpy(&sbuf[4], (byte *)st, len);
		SendTo(&sbuf[0], 4 + len, toid);
	}

	void BotGame::SendTo( ushort op, std::vector<byte>& toid )
	{
		std::vector<byte> sbuf;
		sbuf.resize(4);
		*(ushort *)&sbuf[0] = op;
		*(ushort *)&sbuf[2] = 4;
		SendTo(&sbuf[0], 4, toid);
	}

	void BotGame::SendTo( const byte * buf, size_t len, std::vector<byte>& toid )
	{
		size_t size = toid.size();
		Utils::FastMutex::ScopedLock lock(_playersMutex);
		for(size_t i = 0; i < size; ++ i)
		{
			byte id = toid[i];
			if(_players[id - 1] != 0)
			{
				((Network::TcpClient *)_players[id - 1])->Send(buf, len);
			}
		}
	}

	void BotGame::SetTeam( uint id, byte v )
	{
		if((_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN) || id == 0 || id > 12 || v > 12)
			return;
		if(_gameType == 1)
		{
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			Utils::FastMutex::ScopedLock lock2(_slotsMutex);
			size_t i = id;
			do
			{
				if(i >= _totalSlots)
					i = 0;
				if(_slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OPEN && _slots[i].GetTeam() == v)
				{
					byte opid = _slots[id - 1].GetPID();
					if(opid < 1 || opid > 12 || _players[opid - 1] == 0)
						return;
					if(i >= _mapCfg->GetRawNumPlayers())
						_slots[i] = RefereeSlot;
					else
						_slots[i] = _mapCfg->GetSlots()[i];
					_slots[i].SetPID(opid);
					_slots[i].SetDownloadStatus(_slots[id - 1].GetDownloadStatus());
					_slots[i].SetSlotStatus(GameSlot::SLOTSTATUS_OCCUPIED);
					_slots[i].SetComputer(0);
					_slots[i].SetComputerType(GameSlot::SLOTCOMP_NORMAL);
					_slots[i].SetHandicap(_slots[id - 1].GetHandicap());
					if(id > _mapCfg->GetRawNumPlayers())
						_slots[id - 1] = RefereeSlot;
					else
						_slots[id - 1] = _mapCfg->GetSlots()[id - 1];
					_players[opid - 1]->SetSlotID(i + 1);
					SendSlotInfo();
					return;
				}
				++ i;
			}
			while (i != id);
		}
		else
		{
			Utils::FastMutex::ScopedLock lock(_slotsMutex);
			byte oteam = _slots[id - 1].GetTeam();
			if(oteam == v)
				return;
			if(oteam == 12)
			{
				if(CountNoObPlayers() >= _mapCfg->GetNumPlayers())
					return;
			}
			byte oldColor = _slots[id - 1].GetColor();
			_slots[id - 1].SetTeam(v);
			if(v == 12)
			{
				_slots[id - 1].SetColor(12);
			}
			else if(oldColor == 12)
			{
				byte colort[12] = {0};
				for(size_t i = 0; i < _totalSlots; ++ i)
				{
					if(_slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OCCUPIED && _slots[i].GetColor() < 12)
						colort[_slots[i].GetColor()] = 1;
				}
				for(int i = 0; i < 12; ++ i)
				{
					if(colort[i] == 0)
					{
						_slots[id - 1].SetColor(i);
						break;
					}
				}
			}
			SendSlotInfo();
		}
	}

	void BotGame::SetColor( uint id, byte v )
	{
		if((_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN) || id == 0 || id > 12 || v > 12)
			return;
		{
			Utils::FastMutex::ScopedLock lock(_slotsMutex);
			for(size_t i = 0; i < _totalSlots; ++ i)
			{
				if(_slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OCCUPIED && _slots[i].GetColor() == v)
					return;
			}
			_slots[id - 1].SetColor(v);
		}
		SendSlotInfo();
	}

	void BotGame::SetRace( uint id, byte v )
	{
		if((_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN) || id == 0 || id > 12)
			return;
		switch(v & 63)
		{
		case 1:
		case 2:
		case 4:
		case 8:
		case 32:
			break;
		default:
			return;
		}
		{
			Utils::FastMutex::ScopedLock lock(_slotsMutex);
			_slots[id - 1].SetRace(v);
		}
		SendSlotInfo();
	}

	void BotGame::SetHandicap( uint id, byte v )
	{
		if((_stage != STAGE_PREPARE && _stage != STAGE_COUNTDOWN) || id == 0 || id > 12 || (v != 50 && v != 60 && v != 70 && v != 80 && v != 90 && v != 100))
			return;
		{
			Utils::FastMutex::ScopedLock lock(_slotsMutex);
			_slots[id - 1].SetHandicap(v);
		}
		SendSlotInfo();
	}

	byte BotGame::CountNoObPlayers()
	{
		byte c = 0;
		Utils::FastMutex::ScopedLock lock(_slotsMutex);
		for(size_t i = 0; i < _totalSlots; ++ i)
		{
			if(_slots[i].GetSlotStatus() == GameSlot::SLOTSTATUS_OCCUPIED && _slots[i].GetTeam() < 12)
				++ c;
		}
		return c;
	}

	void BotGame::SendChat( byte oid, std::vector<byte>& toid, byte fromid, uint flag, std::string& chat )
	{
		if(toid.empty() || chat.empty())
			return;
		if(oid > 0 && chat[0] == '!')
		{
			ProcessCommand(oid, chat);
			return;
		}
		if(_stage == STAGE_PREPARE || _stage == STAGE_COUNTDOWN)
		{
			if(flag != 0 || toid.empty())
				return;
			Utils::Stream st;
			st<<(byte)toid.size();
			st.append(&toid[0], toid.size());
			st<<fromid<<(byte)16;
			st<<chat;
			SendToAll(0x0FF7, st, fromid - 1);
		}
		else
		{
			Utils::Stream st;
			st<<(byte)toid.size();
			st.append(&toid[0], toid.size());
			st<<fromid<<(byte)32<<flag;
			st<<chat;
			SendTo(0x0FF7, st, toid);
		}
	}

	void BotGame::ProcessCommand( byte oid, std::string& cmd )
	{
		size_t pos = cmd.find(' ');
		if(pos == std::string::npos)
			ProcessCommand(oid, std::string(cmd.begin() + 1, cmd.end()), std::string());
		else
			ProcessCommand(oid, std::string(cmd.begin() + 1, cmd.begin() + pos), std::string(cmd.begin() + pos + 1, cmd.end()));
	}

	void BotGame::ProcessCommand( byte oid, std::string cmd, std::string arg )
	{
		if(cmd == "start")
		{
			if(_stage != STAGE_PREPARE)
				return;
			SendStartGame();
		}
		else if(cmd == "ready")
		{
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			if(_stage != STAGE_PREPARE || oid < 1 || oid > 12 || _players[oid - 1] == NULL)
				return;
			uint sid = _players[oid - 1]->GetSlotID();
			Utils::FastMutex::ScopedLock lock2(_slotsMutex);
			if(sid > 0 && sid <= _totalSlots && _slots[sid - 1].GetTeam() == 12)
			{
				return;
			}
			if(_players[oid - 1]->GetReady())
			{
				_players[oid - 1]->SetReady(false);
				char chat[1024];
				sprintf(chat, "%s turns to not ready.", _players[oid - 1]->GetName().c_str());
				SendChatToAll(chat);
				_allReadyTime = 0;
				SendNumPlayerInfo();
			}
			else
			{
				_players[oid - 1]->SetReady(true);
				char chat[1024];
				sprintf(chat, "%s is ready.", _players[oid - 1]->GetName().c_str());
				SendChatToAll(chat);
				CheckForReady();
			}
		}
		else if(cmd == "kick")
		{
			std::vector<std::string> plist;
			Text::SeperateString(plist, arg, ' ');
			if(plist.size() < 1)
			{
				SendChatTo(oid, "You should at least set slot for kicking.");
				return;
			}
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			Utils::FastMutex::ScopedLock lock2(_slotsMutex);
			for(size_t i = 0; i < plist.size(); ++ i)
			{
				byte sid = atoi(plist[i].c_str());
				if(sid == 0 || sid > _totalSlots || _slots[sid - 1].GetSlotStatus() != GameSlot::SLOTSTATUS_OCCUPIED || (_slots[sid - 1].GetPID() == 0 && _slots[sid - 1].GetComputer() == 0))
				{
					char txtstr[128];
					sprintf(txtstr, "Wrong slot id: %d.", sid);
					SendChatTo(oid, txtstr);
					continue;
				}
				if(_slots[sid - 1].GetPID() > 0 && _players[_slots[sid - 1].GetPID() - 1] != NULL)
				{
					_players[_slots[sid - 1].GetPID() - 1]->LeaveGame();
				}
				_slots[sid - 1] = _mapCfg->GetSlots()[sid - 1];
			}
		}
		else if(cmd == "setai")
		{
			std::vector<std::string> plist;
			Text::SeperateString(plist, arg, ' ');
			if(plist.size() < 1)
			{
				SendChatTo(oid, "You should at least set slot for ai.");
				return;
			}
			byte ailevel = GameSlot::SLOTCOMP_NORMAL;
			size_t size = plist.size();
			if(size > 1)
			{
				-- size;
				ailevel = atoi(plist[size].c_str());
				if(ailevel == 0 || ailevel > 3)
				{
					ailevel = 3;
				}
				else
					-- ailevel;
			}
			{
				Utils::FastMutex::ScopedLock lock(_slotsMutex);
				for(size_t i = 0; i < size; ++ i)
				{
					byte sid = atoi(plist[i].c_str());
					if(sid == 0 || sid > _totalSlots || (_slots[sid - 1].GetSlotStatus() != GameSlot::SLOTSTATUS_OPEN && _slots[sid - 1].GetComputer() == 0))
					{
						char txtstr[128];
						sprintf(txtstr, "Wrong slot id: %d.", sid);
						SendChatTo(oid, txtstr);
						continue;
					}
					if(ailevel > 2)
					{
						_slots[sid - 1].SetSlotStatus(GameSlot::SLOTSTATUS_OPEN);
						_slots[sid - 1].SetComputer(0);
						_slots[sid - 1].SetComputerType(GameSlot::SLOTCOMP_NORMAL);
					}
					else
					{
						_slots[sid - 1].SetSlotStatus(GameSlot::SLOTSTATUS_OCCUPIED);
						_slots[sid - 1].SetComputer(1);
						_slots[sid - 1].SetComputerType(ailevel);
					}
				}
			}
			SendSlotInfo();
		}
	}

	void BotGame::LoadEnd( uint id )
	{
		if(_stage != STAGE_LOADING)
			return;
		Utils::Stream st;
		st<<(byte)id;
		SendToAll(0x08F7, st);

		{
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			for(uint i = 0; i < 12; ++ i)
			{
				if(_players[i] != 0 && !_players[i]->GetLoadEnd())
				{
					return;
				}
			}
		}

		_stage = STAGE_PLAYING;
		_lastTrigTime = Utils::GetTicks();
		SendAllActions(100);
	}

	void BotGame::SendAllActions(uint ptime)
	{
		if(_stage != STAGE_PLAYING)
			return;
		Crypto::CRC32 crc32;
		Utils::Stream st;
		{
			Utils::FastMutex::ScopedLock lock(_actionsMutex);
			if(_actions.empty())
			{
				st<<(ushort)ptime;
				SendToAll(0x0CF7, st);
				return;
			}
			st.append_zero(4);
			size_t count = _actions.size();
			for(size_t i = 0; i < count; ++ i)
			{
				std::vector<byte>& action = _actions[i];
				if(st.size() + action.size() > 1446)
				{
					*(ushort *)(byte *)st = 0;
					*(ushort *)((byte *)st + 2) = (ushort)crc32.CalcData(((byte *)st) + 4, st.size() - 4);
					SendToAll(0x48F7, st);
					st.clear();
					st.append_zero(4);
				}
				st.append(&action[0], action.size());
			}
			_actions.clear();
		}
		*(ushort *)(byte *)st = (ushort)ptime;
		*(ushort *)((byte *)st + 2) = (ushort)crc32.CalcData(((byte *)st) + 4, st.size() - 4);
		SendToAll(0x0CF7, st);
	}

	void BotGame::PushAction( byte id, const byte * buf, size_t len )
	{
		if(_stage != STAGE_PLAYING)
			return;
		std::vector<byte> action;
		action.resize(3 + len);
		action[0] = id;
		*(ushort *)&action[1] = (ushort)len;
		memcpy(&action[3], buf, len);

		Utils::FastMutex::ScopedLock lock(_actionsMutex);
		_actions.push_back(action);
	}

	void BotGame::CheckPlayersLag()
	{
		uint nMax = 0;
		Utils::FastMutex::ScopedLock lock(_playersMutex);
		for(uint i = 0; i < 12; ++ i)
		{
			if(_players[i] != NULL && _players[i]->GetSyncCounter() > nMax)
				nMax = _players[i]->GetSyncCounter();
		}
		if(_stage == STAGE_LAGGING)
		{
			uint lag = false;
			for(uint i = 0; i < 12; ++ i)
			{
				if(_players[i] != NULL && _players[i]->GetLag() > 0)
				{
					if(nMax < _players[i]->GetSyncCounter() + botCfg.SyncLimit() / 2)
					{
						SendStopLag(_players[i]);
						_players[i]->SetLag(0);
					}
					else if(Utils::GetTicks() - _players[i]->GetLag() >= 35 * 1000)
					{
						_players[i]->SetLeaveReason(1);
						_players[i]->LeaveGame();
					}
					else
						lag = true;
				}
			}
			if(!lag)
			{
				_stage = STAGE_PLAYING;
				_lastTrigTime = Utils::GetTicks();
			}
		}
		else
		{
			bool lag = false;
			for(uint i = 0; i < 12; ++ i)
			{
				if(_players[i] != NULL && nMax > botCfg.SyncLimit() + _players[i]->GetSyncCounter())
				{
					_players[i]->SetLag(Utils::GetTicks());
					lag = true;
				}
			}
			if(lag)
			{
				_stage = STAGE_LAGGING;
				SendStartLag();
			}
		}
	}

	void BotGame::SendStartLag()
	{
		Utils::Stream st;
		st<<(byte)0;
		byte c = 0;
		{
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			for(uint i = 0; i < 12; ++ i)
			{
				if(_players[i] != NULL)
				{
					if(_players[i]->GetLag() > 0)
					{
						st<<(byte)_players[i]->GetPID()<<(Utils::GetTicks() - _players[i]->GetLag());
						++ c;
					}
				}
			}
		}
		if(c == 0)
			return;
		*(byte*)st = c;
		SendToAll(0x10F7, st);
	}

	void BotGame::SendStopLag( Player* pl )
	{
		SendStopLag((byte)pl->GetPID(), pl->GetLag());
	}

	void BotGame::SendStopLag( byte id, uint tick )
	{
		Utils::Stream st;
		st << id << (Utils::GetTicks() - tick);
		SendToAll(0x11F7, st);
	}

	void BotGame::DeCreate()
	{
		BroadcastServer::GetSingleton().Broadcast(6112, 0x33F7, (const byte*)&_id, 4);
	}

	void BotGame::ReCalcUsedSlots()
	{
		uint c = 0;
		Utils::FastMutex::ScopedLock lock(_slotsMutex);
		for(size_t i = 0; i < _totalSlots; ++ i)
		{
			if(_slots[i].GetSlotStatus() != GameSlot::SLOTSTATUS_OPEN)
				++ c;
		}
		LOG_DEBUG(("Used slots: %d", c));
		if(_usedSlots != c)
		{
			_usedSlots = c;
			BroadcastSlotInfo();
		}
	}

	std::string BotGame::GetStageName()
	{
		switch(_stage)
		{
		case STAGE_PREPARE:
			return "Preparing";
		case STAGE_COUNTDOWN:
			return "Loading";
		case STAGE_LOADING:
			return "Loading";
		case STAGE_PLAYING:
			return "Playing";
		case STAGE_LAGGING:
			return "Playing(somebody lagging)";
		case STAGE_FINISHED:
			return "Finished";
		}
		return "";
	}

	void BotGame::SendStartGame()
	{
		SendChatToAll("Game starts, let's rock!");
		SendToAll(0x0AF7);
		_lastTrigTime = Utils::GetTicks();
		_stage = STAGE_COUNTDOWN;
		DeCreate();
	}

	void BotGame::SendChatToAll( std::string chat )
	{
		Utils::Stream st;
		st<<(byte)1<<(byte)0xFF;
		st<<(byte)(1 + _firstPID)<<(byte)16;
		st<<chat;
		SendToAll(0x0FF7, st);
	}

	void BotGame::SendChatTo( byte toid, std::string chat )
	{
		std::vector<byte> toid_;
		toid_.push_back(toid);
		Utils::Stream st;
		st<<(byte)1<<(byte)0xFF;
		st<<(byte)(1 + _firstPID)<<(byte)16;
		st<<chat;
		SendTo(0x0FF7, st, toid_);
	}

	void BotGame::CheckForReady()
	{
		if(CountNoObPlayers() < _mapCfg->GetMinStartCount())
		{
			_allReadyTime = 0;
			return;
		}
		bool rdy = true;
		{
			Utils::FastMutex::ScopedLock lock(_playersMutex);
			Utils::FastMutex::ScopedLock lock2(_slotsMutex);
			for(uint i = 0; i < 12; ++ i)
			{
				if(_players[i] != NULL && !_players[i]->GetReady())
				{
					uint sid = _players[i]->GetSlotID();
					if(sid > 0 && sid <= _totalSlots && _slots[sid - 1].GetTeam() < 12)
					{
						rdy = false;
						break;
					}
				}
			}
		}
		if(rdy)
		{
			SendChatToAll("Everybody is ready, game will start in 5 seconds.");
			_allReadyTime = Utils::GetTicks();
		}
		else
		{
			SendNumPlayerInfo();
		}
	}

	void BotGame::SendNumPlayerInfo()
	{
		uint n = CountNoObPlayers();
		if(n < _mapCfg->GetMinStartCount())
		{
			char chat[1024];
			sprintf(chat, "Current players: %d    We need %d more players to start this game.", n, _mapCfg->GetMinStartCount() - n);
			SendChatToAll(chat);
		}
		else
		{
			SendChatToAll("Game is ready to start once everybody has typed !ready.");
		}
	}

	void BotGame::SendWelcome(uint id)
	{
		SendChatTo(id, "Welcome to Autobot game!");
		SendChatTo(id, "Type !ready to toggle your status ready for game.");
	}

	void BotGame::BroadcastSlotInfo()
	{
		uint buf[3];
		buf[0] = _id;
		GetSlotStatus(buf[1], buf[2]);
		BroadcastServer::GetSingleton().Broadcast(6112, 0x32F7, (const byte *)&buf[0], 12);
	}
}
