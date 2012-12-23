#ifndef _SAVEDGAME_H_
#define _SAVEDGAME_H_

#include "PackedFile.h"
#include "GameSlot.h"

namespace HostBot
{
	class SavedGame:
		public PackedFile
	{
	public:
		bool Parse();
		inline std::string GetMapName() {return _mapName;}
		inline std::vector<GameSlot>& GetSlots() {return _slots;}
		inline uint GetStageChecksum() {return _stageChecksum;}
		inline std::string GetStats() {return _stats;}
		inline byte GetSelectMode() {return _selectMode;}
	private:
		std::string _mapName;
		std::vector<GameSlot> _slots;
		uint _stageChecksum;
		std::string _stats;
		byte _selectMode;
	};
}

#endif // _SAVEDGAME_H_
