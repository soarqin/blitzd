#include "Config.h"
#include "Player.h"
#include "BotGame.h"
#include "utils/SysTime.h"

namespace HostBot
{
	Player::Player(int s): BotReceiver(s)
	{
		_lastTrigTime = Utils::GetTicks() - 15000;
		_pId = _slotId = 0;
		_lag = 0;
		_host = NULL;
		_loadend = false;
		_syncCounter = 0;
		_ready = false;
		_leaveReason = 0;
		_pingtick = 0;
		_ping = -1;
	}

	void Player::LeaveGame()
	{
		Send(0x1BF7);
		if(!_leaveReason == 0)
		{
			if(_host != NULL)
			{
				if(_host->GetStage() == BotGame::STAGE_PREPARE)
					_leaveReason = 13;
				else
					_leaveReason = 1;
			}
			else
				return;
		}
		ForceLeave();
	}

	Player::~Player()
	{
		LOG_DEBUG(("Killing player"));
		ForceLeave();
		PlayerPool::GetSingleton().Remove(GetSession());
	}

	void Player::BuildPlayerInfo( Utils::Stream& st, byte id )
	{
		st<<(uint)2<<id<<_name<<(ushort)1<<_addr<<_addr;
	}

	bool Player::SetMapProgress( uint prog )
	{
		if(_host != NULL)
			return _host->SetMapProgress( _pId, prog );
		return true;
	}

	void Player::TimerCheck(uint tick)
	{
		if(_host == NULL)
			return;
		if((_host->GetStage() == BotGame::STAGE_PREPARE || _host->GetStage() == BotGame::STAGE_LOADING) && tick - _lastTrigTime >= 30000)
		{
			Utils::Stream st;
			st<<Utils::GetUpTicks();
			Send(0x01F7, st);
			_lastTrigTime = tick;
		}
	}

	void Player::GlobalChat( std::vector<byte>& toid, byte fromid, std::string& chat )
	{
		if(_host == NULL)
			return;
		_host->SendChat(_pId, toid, fromid, 0, chat);
	}

	void Player::InGameChat( std::vector<byte>& toid, byte fromid, uint flag, std::string& chat )
	{
		if(_host == NULL)
			return;
		_host->SendChat(_pId, toid, fromid, flag, chat);
	}

	void Player::SetTeam( byte v )
	{
		if(_host != NULL)
			_host->SetTeam(_slotId, v);
	}

	void Player::SetColor( byte v )
	{
		if(_host != NULL)
			_host->SetColor(_slotId, v);
	}

	void Player::SetRace( byte v )
	{
		if(_host != NULL)
			_host->SetRace(_slotId, v);
	}

	void Player::SetHandicap( byte v )
	{
		if(_host != NULL)
			_host->SetHandicap(_slotId, v);
	}

	void Player::LoadEnd()
	{
		_loadend = true;
		if(_host != NULL)
			_host->LoadEnd(_pId);
	}

	void Player::PushAction( const byte * buf, size_t len )
	{
		if(_host != NULL)
			_host->PushAction(_pId, buf, len);
	}

	void Player::PushSync()
	{
		++ _syncCounter;
	}

	void Player::ForceLeave()
	{
		if(_host != NULL)
		{
			_host->Kick(GetSession(), _leaveReason);
			_host = NULL;
		}
		_slotId = 0;
	}

	void Player::OnDisconnect()
	{
		SetLeaveReason(1);
		LeaveGame();
	}

	void PlayerPool::Add( Player * ptr )
	{
		Utils::FastMutex::ScopedLock lock(_p_mutex);
		std::map<uint, Player *>::iterator it = _players.find(ptr->GetSession());
		if(it == _players.end())
			_players[ptr->GetSession()] = ptr;
		else
		{
			it->second->LeaveGame();
			_players[ptr->GetSession()] = ptr;
		}
	}

	void PlayerPool::Remove( uint s )
	{
		Utils::FastMutex::ScopedLock lock(_p_mutex);
		_players.erase(s);
	}

	Player * PlayerPool::operator[]( uint s )
	{
		Utils::FastMutex::ScopedLock lock(_p_mutex);
		std::map<uint, Player *>::iterator it = _players.find(s);
		if(it == _players.end())
			return NULL;
		return it->second;
	}

	void PlayerPool::TimerCheck(uint tick)
	{
		Utils::FastMutex::ScopedLock lock(_p_mutex);
		std::map<uint, Player *>::iterator it;
		for(it = _players.begin(); it != _players.end(); ++ it)
		{
			if(it->second)
				it->second->TimerCheck(tick);
		}
	}
}
