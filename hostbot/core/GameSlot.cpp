#include "Config.h"
#include "GameSlot.h"

namespace HostBot
{
	GameSlot::GameSlot( std::vector<byte> &n )
	{
		if( n.size( ) >= 7 )
		{
			_PID = n[0];
			_DownloadStatus = n[1];
			_SlotStatus = n[2];
			_Computer = n[3];
			_Team = n[4];
			_Color = n[5];
			_Race = n[6];
	
			if( n.size( ) >= 8 )
				_ComputerType = n[7];
			else
				_ComputerType = SLOTCOMP_NORMAL;
	
			if( n.size( ) >= 9 )
				_Handicap = n[8];
			else
				_Handicap = 100;
		}
		else
		{
			_PID = 0;
			_DownloadStatus = 255;
			_SlotStatus = SLOTSTATUS_OPEN;
			_Computer = 0;
			_Team = 0;
			_Color = 1;
			_Race = SLOTRACE_RANDOM;
			_ComputerType = SLOTCOMP_NORMAL;
			_Handicap = 100;
		}
	}
	
	GameSlot::GameSlot( byte nPID, byte nDownloadStatus, byte nSlotStatus, byte nComputer, byte nTeam, byte nColor, byte nRace, byte nComputerType, byte nHandicap )
	{
		_PID = nPID;
		_DownloadStatus = nDownloadStatus;
		_SlotStatus = nSlotStatus;
		_Computer = nComputer;
		_Team = nTeam;
		_Color = nColor;
		_Race = nRace;
		_ComputerType = nComputerType;
		_Handicap = nHandicap;
	}

	GameSlot::GameSlot()
	{
		_PID = 0;
		_DownloadStatus = 0xFF;
		_SlotStatus = 0;
		_Computer = 0;
		_Team = 0;
		_Color = 0;
		_Race = 0;
		_ComputerType = 1;
		_Handicap = 100;
	}

	GameSlot::~GameSlot( )
	{
	
	}
	
	std::vector<byte> GameSlot::GetArray( )
	{
		std::vector<byte> b;
		b.push_back( _PID );
		b.push_back( _DownloadStatus );
		b.push_back( _SlotStatus );
		b.push_back( _Computer );
		b.push_back( _Team );
		b.push_back( _Color );
		b.push_back( _Race );
		b.push_back( _ComputerType );
		b.push_back( _Handicap );
		return b;
	}

	void GameSlot::GetSlotData( Utils::Stream& st )
	{
		st<<_PID;
		st<<_DownloadStatus;
		st<<_SlotStatus;
		st<<_Computer;
		st<<_Team;
		st<<_Color;
		st<<_Race;
		st<<_ComputerType;
		st<<_Handicap;
	}
}
