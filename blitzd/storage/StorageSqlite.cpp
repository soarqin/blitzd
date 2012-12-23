#include "Config.h"
#include "StorageSqlite.h"
#include "utils/NumberFormatter.h"

namespace Storage
{
	extern StorageSqlite storageSqlite;

	bool StorageSqlite::Open( const char * filename )
	{
		if(sqlite3_open(filename, &_sqlite) == SQLITE_OK)
		{
			return true;
		}
		return false;
	}

	void StorageSqlite::Close()
	{
		if(_sqlite != NULL)
		{
			sqlite3_close(_sqlite);
			_sqlite = NULL;
		}
	}

	bool StorageSqlite::Create( const char * tablename, StructDef * structs )
	{
		if(_sqlite == NULL || structs == 0)
		{
			return false;
		}
		std::string query = "CREATE TABLE IF NOT EXISTS ";
		query = query + tablename + " (";
		std::string keys = "";
		std::string unis = "";
		for(uint i = 0; structs[i].name != NULL; i ++)
		{
			if(i > 0)
				query += ", ";
			query = query + structs[i].name + ' ' + getTypeName(structs[i].type);
			if((structs[i].option & 3) == 1)
			{
				if(keys.length() > 0)
					keys += ", ";
				keys += structs[i].name;
			}
			else if((structs[i].option & 3) == 2)
			{
				if(unis.length() > 0)
					unis += ", ";
				unis += structs[i].name;
			}
			else if((structs[i].option & 4) == 4)
			{
				query += " NOT NULL";
			}
		}
		if(keys.length() > 0)
		{
			query += ", PRIMARY KEY(";
			query += keys;
			query += ")";
		}
		if(unis.length() > 0)
		{
			query += ", UNIQUE(";
			query += unis;
			query += ")";
		}
		query += ")";
		return doExec(query.c_str());
	}

	const char * StorageSqlite::getTypeName( Storage::DataType t )
	{
		switch(t)
		{
		case Storage::Int:
			return "INTEGER(20)";
		case Storage::Real:
			return "REAL";
		case Storage::String:
			return "TEXT";
		default:
			return "BLOB";
		}
		return "BLOB";
	}

	bool StorageSqlite::doExec( const char* exec )
	{
		return sqlite3_exec(_sqlite, exec, NULL, NULL, NULL) == SQLITE_OK;
	}

	bool StorageSqlite::doQuery( const char* query, uint col, std::vector<std::vector<DataDef> >& result)
	{
		sqlite3_stmt * stmt;
		const char * tail;
		if(sqlite3_prepare_v2(_sqlite, query, -1, &stmt, &tail) == SQLITE_OK)
		{
			while(sqlite3_step(stmt) == SQLITE_ROW)
			{
				result.resize(result.size() + 1);
				std::vector<DataDef>& row = result.back();
				row.resize(col);
				for(uint i = 0; i < col; i ++)
				{
					switch(sqlite3_column_type(stmt, i))
					{
					case SQLITE_NULL:
						row[i].SetInt(0L);
						break;
					case SQLITE_INTEGER:
						row[i].SetInt(sqlite3_column_int64(stmt, i));
						break;
					case SQLITE_FLOAT:
						row[i].SetReal(sqlite3_column_double(stmt, i));
						break;
					case SQLITE_TEXT:
						{
							row[i].type = Storage::String;
							uint size = sqlite3_column_bytes(stmt, i);
							if(size > 0)
							{
								row[i].SetString((const char *)sqlite3_column_text(stmt, i));
							}
							else
							{
								row[i].SetString("");
							}
						}
						break;
					default:
						{
							uint size = sqlite3_column_bytes(stmt, i);
							if(size > 0)
							{
								row[i].SetBinary((const byte *)sqlite3_column_blob(stmt, i), size);
							}
							else
							{
								row[i].bVal = NULL;
								row[i].bSize = 0;
							}
						}
						break;
					}
				}
			}
			return true;
		}
		return false;
	}

	uint StorageSqlite::Set( const char * tablename, const char ** colnames, DataDef * data )
	{
		bool result = true;
		std::string query = "REPLACE INTO ";
		query = query + tablename + " (";
		uint i;
		for(i = 0; colnames[i] != NULL; i ++)
		{
			if(i > 0)
				query += ", ";
			query += colnames[i];
		}
		query += ") VALUES (";
		uint len = i;
		for(uint j = 0; j < len; j ++)
		{
			if(j > 0)
				query += ", ";
			switch(data[j].type)
			{
			case Storage::Int:
				Utils::NumberFormatter::append(query, (Utils::Int64)data[j].lVal);
				break;
			case Storage::Real:
				Utils::NumberFormatter::append(query, data[j].rVal, 2);
				break;
			case Storage::String:
				query += '"';
				query += data[j].sVal;
				query += '"';
				break;
			default:
				query += "X'";
				uint bSize = data[j].bSize;
				for(uint k = 0; k < bSize; k ++)
				{
					Utils::NumberFormatter::appendHex(query, data[j].bVal[k], 2);
				}
				query += "'";
				break;
			}
		}
		query += ")";
		result = doExec(query.c_str());
		return result;
	}

	bool StorageSqlite::GetAll( const char * tablename, const char ** colnames, std::vector<std::vector<DataDef> >& result )
	{
		std::string query = "SELECT ";
		uint i;
		for(i = 0; colnames[i] != NULL; i ++)
		{
			if(i > 0)
				query += ", ";
			query += colnames[i];
		}
		query += " FROM ";
		query += tablename;
		return doQuery(query.c_str(), i, result);
	}

	bool StorageSqlite::GetMatch( const char * tablename, const char ** colnames, const char * matchcond, std::vector<std::vector<DataDef> >& result )
	{
		if(matchcond == NULL || matchcond[0] == 0)
		{
			return GetAll(tablename, colnames, result);
		}
		std::string query = "SELECT ";
		uint i;
		for(i = 0; colnames[i] != NULL; i ++)
		{
			if(i > 0)
				query += ", ";
			query += colnames[i];
		}
		query += " FROM ";
		query += tablename;
		query += " WHERE ";
		query += matchcond;
		return doQuery(query.c_str(), i, result);
	}

	void StorageSqlite::RemoveAll( const char * tablename )
	{
		std::string query = "DELETE FROM ";
		query += tablename;
		doExec(query.c_str());
	}

	void StorageSqlite::RemoveMatch( const char * tablename, const char * cond )
	{
		if(cond == NULL || cond[0] == 0)
		{
			RemoveAll(tablename);
			return;
		}
		std::string query = "DELETE FROM ";
		query += tablename;
		query += " WHERE ";
		query += cond;;
		doExec(query.c_str());
	}

	void StorageSqlite::Begin()
	{
		doExec("BEGIN");
	}

	void StorageSqlite::Commit()
	{
		doExec("COMMIT");
	}

	void StorageSqlite::Optimize()
	{
		doExec("VACUUM");
	}
}
