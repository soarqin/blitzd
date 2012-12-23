#include "Config.h"
#include "SavedGame.h"

namespace HostBot
{
	bool SavedGame::Parse()
	{
		std::string dummy, gameName;
		_data >> _mapName >> dummy >> gameName >> dummy >> _stats;
		_data += 4;	// Num players
		_data += 4;	// Game type
		_data += 2;	// length of slots
		byte numSlots;
		_data >> numSlots;	// Num slots
		if(numSlots > 12)
			return false;
		_slots.resize(numSlots);
		for(byte i = 0; i < numSlots; ++ i)
		{
			_data >> _slots[i];
		}
		_data += 4;
		_data >> _selectMode;
		_data += 1;
		_data >> _stageChecksum;
		return true;
	}
}
