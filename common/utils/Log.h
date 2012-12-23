#ifndef _LOG_H_
#define _LOG_H_

#ifdef ENABLE_LOG

#ifndef LOG_LEVEL
#define LOG_LEVEL 2
#endif

#include "Singleton.h"

namespace Utils
{
#ifndef LOG_FILENAME
#define LOG_FILENAME NULL
#endif

#define LOG_TIME_FORMAT "%b %d %H:%M:%S"
#define LOG_TIME_FORMAT_W L"%b %d %H:%M:%S"
#define LOG_TIME_MAXLEN 32
	class Log:
		public Singleton<Log>
	{
	public:
		typedef enum {
			LOG_TRACE,
			LOG_DEBUG,
			LOG_INFO,
			LOG_ERROR
		} Type_t;
		Log(const char * szFilename = NULL);
		~Log();
		void Init();
		void Free();
#ifndef LOG_NO_VERBOSE
		void DoLog(Type_t t, const char * module, int line, const char * str);
#else
		void DoLog(Type_t t, const char * str);
#endif
#ifndef LOG_NO_VERBOSE
		void DoLog(Type_t t, const char * module, int line, const byte * data, int size);
#else
		void DoLog(Type_t t, const byte * data, int size);
#endif
		static char * DoFormat(const char * fmt, ...);
		static char * DoFormat(const byte * data, int size);
	private:
#ifdef WIN32
		HANDLE m_fout;
#else
		FILE * m_fout;
#endif
		std::string m_filename;

		inline const char * _LogType(Type_t t)
		{
			switch(t)
			{
			case LOG_DEBUG:
				return "[DEBUG]";
			case LOG_TRACE:
				return "[TRACE]";
			case LOG_INFO:
				return "[INFO ]";
			case LOG_ERROR:
				return "[ERROR]";
			default:
				return "";
			}
		}
	};
}

#ifdef LOG_NO_VERBOSE

#if LOG_LEVEL > 3
#define LOG_TRACE(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_TRACE, Utils::Log::DoFormat V)
#endif
#if LOG_LEVEL > 2
#define LOG_DEBUG(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_DEBUG, Utils::Log::DoFormat V)
#endif
#if LOG_LEVEL > 1
#define LOG_INFO(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_INFO, Utils::Log::DoFormat V)
#endif
#if LOG_LEVEL > 0
#define LOG_ERROR(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_ERROR, Utils::Log::DoFormat V)
#endif

#else

#if LOG_LEVEL > 3
#define LOG_TRACE(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_TRACE, __FUNCTION__, __LINE__, Utils::Log::DoFormat V)
#endif
#if LOG_LEVEL > 2
#define LOG_DEBUG(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_DEBUG, __FUNCTION__, __LINE__, Utils::Log::DoFormat V)
#endif
#if LOG_LEVEL > 1
#define LOG_INFO(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_INFO, __FUNCTION__, __LINE__, Utils::Log::DoFormat V)
#endif
#if LOG_LEVEL > 0
#define LOG_ERROR(V) Utils::Log::GetSingleton().DoLog(Utils::Log::LOG_ERROR, __FUNCTION__, __LINE__, Utils::Log::DoFormat V)
#endif

#endif

#if LOG_LEVEL < 4
#define LOG_TRACE(V)
#endif

#if LOG_LEVEL < 3
#define LOG_DEBUG(V)
#endif

#if LOG_LEVEL < 2
#define LOG_INFO(V)
#endif

#if LOG_LEVEL < 1
#define LOG_ERROR(V)
#endif

#else

#define LOG_TRACE(V)
#define LOG_DEBUG(V)
#define LOG_INFO(V)
#define LOG_ERROR(V)

#endif

#endif // _LOG_H_
