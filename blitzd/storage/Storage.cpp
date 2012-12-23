#include "Config.h"
#include "Storage.h"
#include "StorageSqlite.h"
#include "utils/Memory.h"

namespace Storage
{
	Storage * storage = NULL;

	void Storage::SetEngine( const char * name )
	{
		if(_STR.strcmpi(name, "sqlite") == 0)
		{
			storage = &storageSqlite.GetSingleton();
		}
	}
}
