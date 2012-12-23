#ifndef _STORAGESQLITE_H_
#define _STORAGESQLITE_H_

#include "Storage.h"
#include "utils/Singleton.h"

#include "sqlite3/sqlite3.h"

namespace Storage
{
	class StorageSqlite:
		public Utils::Singleton<StorageSqlite>,
		public Storage
	{
	public:
		virtual bool Open(const char *);
		virtual void Close();
		virtual bool Create(const char *, StructDef *);
		virtual void Begin();
		virtual void Commit();
		virtual void Optimize();
		virtual uint Set(const char *, const char **, DataDef *);
		virtual bool GetAll(const char *, const char **, std::vector<std::vector<DataDef> >&);
		virtual bool GetMatch(const char *, const char **, const char *, std::vector<std::vector<DataDef> >&);
		virtual void RemoveAll(const char *);
		virtual void RemoveMatch(const char *, const char *);
	private:
		sqlite3 * _sqlite;

		const char * getTypeName(DataType);
		bool doExec(const char*);
		bool doQuery(const char*, uint, std::vector<std::vector<DataDef> >&);
	};
	extern StorageSqlite storageSqlite;
}

#endif // _STORAGESQLITE_H_
