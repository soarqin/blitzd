#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "utils/Memory.h"

namespace Storage
{
	class Storage
	{
	public:
		enum DataType
		{
			Unknown,
			Int,
			Real,
			String,
			Binary
		};
		enum DataOption
		{
			PrimKey = 1,
			Unique = 2,
			NotNull = 4
		};
		struct StructDef
		{
			char* name;
			DataType type;
			byte option;
		};
		struct DataDef
		{
			DataDef(): type(Unknown) {}
			DataDef(const DataDef& _d)
			{
				switch(_d.type)
				{
				case Unknown:
					type = Unknown;
					break;
				case String:
					if(_d.sVal != NULL)
					{
						SetString(_d.sVal);
					}
					else
					{
						SetString("");
					}
					break;
				case Binary:
					if(bVal != NULL)
					{
						SetBinary(_d.bVal, _d.bSize);
					}
					else
					{
						type = Binary;
						bVal = NULL;
						bSize = 0;
					}
					break;
				case Int:
					SetInt(_d.lVal);
					break;
				case Real:
					SetReal(_d.rVal);
					break;
				default:
					type = Unknown;
					break;
				}
			}
			~DataDef()
			{
				if(type == String)
				{
					if(sVal != NULL)
					{
						_MEM.Free(sVal);
						sVal = NULL;
					}
				}
				else if(type == Binary)
				{
					if(bVal != NULL)
					{
						_MEM.Free(bVal);
						bVal = NULL;
					}
				}
			}
			void SetString(const char * str)
			{
				type = String;
				sSize = _STR.strlen(str);
				sVal = (char *)_MEM.Alloc(sSize + 1);
				memcpy(sVal, str, sSize + 1);
			}
			void SetBinary(const byte * data, size_t size)
			{
				type = Binary;
				bSize = size;
				if(bSize > 0)
				{
					bVal = (byte *)_MEM.Alloc(bSize);
					memcpy(bVal, data, bSize);
				}
				else
				{
					bVal = NULL;
					bSize = 0;
				}
			}
			void SetInt(int64 l)
			{
				type = Int;
				lVal = l;
			}
			void SetReal(double r)
			{
				type = Real;
				rVal = r;
			}
			DataType type;
			union
			{
				int64 lVal;
				double rVal;
				struct  
				{
					char * sVal;
					uint sSize;
				};
				struct
				{
					byte * bVal;
					uint bSize;
				};
			};
		};

	public:
		static void SetEngine(const char * name);
		virtual bool Open(const char *) = 0;
		virtual void Close() = 0;
		virtual bool Create(const char *, StructDef *) = 0;
		virtual void Begin() = 0;
		virtual void Commit() = 0;
		virtual void Optimize() = 0;
		virtual uint Set(const char *, const char **, DataDef *) = 0;
		virtual bool GetAll(const char *, const char **, std::vector<std::vector<DataDef> >&) = 0;
		virtual bool GetMatch(const char *, const char **, const char *, std::vector<std::vector<DataDef> >&) = 0;
		virtual void RemoveAll(const char *) = 0;
		virtual void RemoveMatch(const char *, const char *) = 0;
	};

	extern Storage * storage;
}

#endif // _STORAGE_H_
