#ifndef _FILE_H_
#define _FILE_H_

namespace Utils
{
	class BasicFileStream
	{
	public:
		enum SeekType
		{
			Begin = 0,
			Current = 1,
			End = 2
		};
		virtual ~BasicFileStream() {}
		virtual bool Open(std::string filename, bool canwrite = false, bool append = false, bool nocache = false) = 0;
		virtual void Close() = 0;
		virtual void Truncate() = 0;
		virtual uint64 Size() = 0;
		virtual uint64 Pos() = 0;
		virtual void Pos(uint64 pos, SeekType type) = 0;
		virtual uint Read(void * data, uint len) = 0;
		virtual uint Write(const void * data, uint len) = 0;
		template<typename T>
		__inline BasicFileStream& operator <<(const T& _Val)
		{
			Write(&_Val, sizeof(T));
			return *this;
		}
		template<typename T>
		__inline BasicFileStream& operator >>(T& _Val)
		{
			Read(&_Val, sizeof(T));
			return *this;
		}
	};

#ifdef WIN32
	class FileStreamWin32: public BasicFileStream
	{
	public:
		FileStreamWin32();
		virtual ~FileStreamWin32();
		virtual bool Open(std::string filename, bool canwrite = false, bool append = false, bool nocache = false);
		virtual void Close();
		virtual void Truncate();
		virtual uint64 Size();
		virtual uint64 Pos();
		virtual void Pos(uint64 pos, SeekType type);
		virtual uint Read(void * data, uint len);
		virtual uint Write(const void * data, uint len);
	private:
		HANDLE _file;
	};

	typedef FileStreamWin32 FileStream;

#else

	class FileStreamGeneric: public BasicFileStream
	{
	public:
		FileStreamGeneric();
		virtual ~FileStreamGeneric();
		virtual bool Open(std::string filename, bool canwrite = false, bool append = false, bool nocache = false);
		virtual void Close();
		virtual void Truncate();
		virtual uint64 Size();
		virtual uint64 Pos();
		virtual void Pos(uint64 pos, SeekType type);
		virtual uint Read(void * data, uint len);
		virtual uint Write(const void * data, uint len);
	private:
		bool _nocache;
		FILE * _file;
	};

	typedef FileStreamGeneric FileStream;
#endif
}

#endif // _FILE_H_
