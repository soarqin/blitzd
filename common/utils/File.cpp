#include "Config.h"
#include "File.h"

namespace Utils
{
#ifdef WIN32
	FileStreamWin32::FileStreamWin32()
	{
		_file = NULL;
	}

	FileStreamWin32::~FileStreamWin32()
	{
		Close();
	}

	bool FileStreamWin32::Open( std::string filename, bool canwrite /*= false*/, bool append /*= false*/, bool nocache /*= false*/ )
	{
		_file = CreateFileA(filename.c_str(), (canwrite ? GENERIC_WRITE : 0) | GENERIC_READ, FILE_SHARE_READ, NULL, canwrite ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | (nocache ? FILE_FLAG_WRITE_THROUGH : 0), NULL);
		if(_file == NULL || _file == INVALID_HANDLE_VALUE)
		{
			_file = NULL;
			return false;
		}
		if(append)
		{
			SetFilePointer(_file, 0, NULL, FILE_END);
		}
		return true;
	}

	void FileStreamWin32::Close()
	{
		if(_file != NULL)
		{
			CloseHandle(_file);
			_file = NULL;
		}
	}

	void FileStreamWin32::Truncate()
	{
		SetEndOfFile(_file);
	}

	uint64 FileStreamWin32::Size()
	{
		DWORD _low, _high;
		_low = GetFileSize(_file, &_high);
		return (uint64)_low + (((uint64)_high) << 32);
	}

	uint64 FileStreamWin32::Pos()
	{
		DWORD _low;
		LONG _high = 0;
		_low = SetFilePointer(_file, 0, &_high, FILE_CURRENT);
		return (uint64)_low + (((uint64)_high) << 32);
	}

	void FileStreamWin32::Pos( uint64 pos, SeekType type )
	{
		LONG _high = (LONG)(pos >> 32);
		SetFilePointer(_file, (LONG)(pos & 0xFFFFFFFF), (_high > 0 ? &_high : NULL), type);
	}

	uint FileStreamWin32::Read( void * data, uint len )
	{
		DWORD size;
		if(ReadFile(_file, data, len, &size, NULL))
			return size;
		else
			return (uint)-1;
	}

	uint FileStreamWin32::Write( const void * data, uint len )
	{
		DWORD size;
		if(WriteFile(_file, data, len, &size, NULL))
			return size;
		else
			return (uint)-1;
	}
#else
	FileStreamGeneric::FileStreamGeneric()
	{
		_nocache = false;
	}

	FileStreamGeneric::~FileStreamGeneric()
	{
		Close();
	}

	bool FileStreamGeneric::Open( std::string filename, bool canwrite /*= false*/, bool append /*= false*/, bool nocache /*= false*/ )
	{
		_nocache = nocache;
		_file = fopen(filename.c_str(), canwrite ? (append ? "a+b" : "r+b") : "rb");
		if(_file == NULL)
		{
			if(canwrite)
				_file = fopen(filename.c_str(), "w+b");
			if(_file == NULL)
				return false;
		}
		return true;
	}

	void FileStreamGeneric::Close()
	{
		if(_file != NULL)
		{
			fclose(_file);
			_file = NULL;
		}
	}

	void FileStreamGeneric::Truncate()
	{

	}

	uint64 FileStreamGeneric::Size()
	{
		uint _pos = (uint)ftell(_file);
		fseek(_file, 0, SEEK_END);
		uint64 _size = (uint64)ftell(_file);
		fseek(_file, _pos, SEEK_SET);
		return _size;
	}

	uint64 FileStreamGeneric::Pos()
	{
		return (uint64)ftell(_file);
	}

	void FileStreamGeneric::Pos( uint64 pos, SeekType type )
	{
		fseek(_file, (uint)pos, type);
	}

	uint FileStreamGeneric::Read( void * data, uint len )
	{
		return fread(data, 1, len, _file);
	}

	uint FileStreamGeneric::Write( const void * data, uint len )
	{
		if(_nocache)
			return fwrite(data, 1, len, _file);
		else
		{
			uint size = fwrite(data, 1, len, _file);
			fflush(_file);
			return size;
		}
	}
#endif
}
