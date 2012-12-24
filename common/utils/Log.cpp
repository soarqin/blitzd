#include "Config.h"
#include "Log.h"
#include <string.h>
#include <stdarg.h>

#ifdef ENABLE_LOG
namespace Utils
{

	Log::Log( const char * szFilename /*= NULL*/ )
	{
		if(szFilename == NULL)
			m_filename.clear();
		else
			m_filename = szFilename;
		Init();
	}

	Log::~Log()
	{
		Free();
	}

	void Log::Init()
	{
		if(m_filename.empty())
		{
			m_filename.clear();
#ifdef _WIN32
/*			AllocConsole();
			CONSOLE_SCREEN_BUFFER_INFO coninfo;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
			coninfo.dwSize.Y = 2000;
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);*/

			m_fout = CreateFileA("CONOUT$", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_WRITE_THROUGH*/, NULL);
#else
			m_fout = stdout;
			setvbuf(m_fout, NULL, _IONBF, 0);
#endif
		}
		else
		{
#ifdef _WIN32
			m_fout = (FILE *)CreateFileA(m_filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
			SetFilePointer(m_fout, 0, NULL, FILE_END);
#else
			m_fout = fopen(m_filename.c_str(), "a+t");
			setvbuf(m_fout, NULL, _IONBF, 0);
#endif
		}
	}

	void Log::Free()
	{
		if(m_filename.empty())
		{
#ifdef _WIN32
			CloseHandle(m_fout);
			FreeConsole();
#endif
		}
		else
		{
#ifdef _WIN32
			CloseHandle(m_fout);
#else
			fclose(m_fout);
#endif
		}
	}

#ifndef LOG_NO_VERBOSE
	void Log::DoLog(Type_t t, const char * module, int line, const char * str)
#else
	void Log::DoLog(Type_t t, const char * str)
#endif
	{
		char time_string[LOG_TIME_MAXLEN];
		struct tm * tmnow;
		time_t      now;

		time(&now);
		if (!(tmnow = localtime(&now)))
			strcpy(time_string, "?");
		else
			strftime(time_string, LOG_TIME_MAXLEN, LOG_TIME_FORMAT, tmnow);

#ifdef _WIN32
		char strTemp[65536];
		DWORD dwDone;
#ifndef LOG_NO_VERBOSE
		int size = wsprintfA(strTemp, "%s %s %s[%d]: ", time_string, _LogType(t), module, line);
#else
		int size = wsprintfA(strTemp, "%s %s: ", time_string, _LogType(t));
#endif
		WriteFile(m_fout, strTemp, size, &dwDone, NULL);
		WriteFile(m_fout, str, lstrlenA(str), &dwDone, NULL);
		WriteFile(m_fout, "\r\n", 2, &dwDone, NULL);
#else
#ifndef LOG_NO_VERBOSE
		fprintf(m_fout, "%s %s %s[%d]: ", time_string, _LogType(t), module, line);
#else
		fprintf(m_fout, "%s %s: ", time_string, _LogType(t));
#endif
		fprintf(m_fout, "%s", str);
		fprintf(m_fout, "\n");
#endif
	}

#ifndef LOG_NO_VERBOSE
	void Log::DoLog(Type_t t, const char * module, int line, const byte * data, int size)
#else
	void Log::DoLog(Type_t t, const byte * data, int size)
#endif
	{
		char time_string[LOG_TIME_MAXLEN];
		struct tm * tmnow;
		time_t      now;

		time(&now);
		if (!(tmnow = localtime(&now)))
			strcpy(time_string, "?");
		else
			strftime(time_string, LOG_TIME_MAXLEN, LOG_TIME_FORMAT, tmnow);

#ifdef _WIN32
		char strTemp[65536];
		DWORD dwDone;
#ifndef LOG_NO_VERBOSE
		int _size = wsprintfA(strTemp, "%s %s %s[%d]: ", time_string, _LogType(t), module, line);
#else
		int _size = wsprintfA(strTemp, "%s %s: ", time_string, _LogType(t));
#endif
		WriteFile(m_fout, strTemp, _size, &dwDone, NULL);
		int i;
		for(i = 0; i < size; ++ i)
		{
			if(i > 0 && i % 16 == 0)
				WriteFile(m_fout, "\r\n ", 3, &dwDone, NULL);
			wsprintfA(strTemp, " %02X", data[i]);
			WriteFile(m_fout, strTemp, 3, &dwDone, NULL);
		}
		WriteFile(m_fout, "\r\n", 2, &dwDone, NULL);
#else
#ifndef LOG_NO_VERBOSE
		fprintf(m_fout, "%s %s %s[%d]:\n ", time_string, _LogType(t), module, line);
#else
		fprintf(m_fout, "%s %s:\n ", time_string, _LogType(t));
#endif

		int i;
		for(i = 0; i < size; ++ i)
		{
			if(i > 0 && i % 16 == 0)
				fprintf(m_fout, "\n ");
			fprintf(m_fout, " %02X", data[i]);
		}
		fprintf(m_fout, "\n");
#endif
	}

	char * Log::DoFormat( const char * fmt, ... )
	{
		va_list args;
		static char strTemp[65536];
		va_start(args, fmt);
		vsprintf(strTemp, fmt, args);
		va_end(args);
		return strTemp;
	}

	char * Log::DoFormat( const byte * data, int size )
	{
		static char strTemp[65536];
		int i;
		strTemp[0] = 0;
		for(i = 0; i < size; ++ i)
		{
#ifdef _WIN32
			wsprintfA(&strTemp[3 * i], "%02X ", data[i]);
#else
			sprintf(&strTemp[3 * i], "%02X ", data[i]);
#endif
		}
		return strTemp;
	}
}
#endif
