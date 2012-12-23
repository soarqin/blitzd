#ifndef _STREAM_H_
#define _STREAM_H_

#include "Memory.h"
#include "SharedPtr.h"
#include <memory.h>

namespace Utils
{
	class Stream:
		public Utils::SharedClass<Stream>
	{
	public:
		Stream(): _pos(0) {}
		Stream(const byte * buf, size_t len): _pos(0)
		{
			if(len > 0)
			{
				_buf.assign(buf, buf + len);
			}
		}
		Stream(std::vector<byte>& v): _pos(0)
		{
			_buf.assign(v.begin(), v.end());
		}
		inline operator std::vector<byte>&() {return _buf;}
		inline operator byte*() {return _buf.empty() ? NULL : &_buf[0];}
		inline size_t size() {return _buf.size();}
		inline size_t left() {return _buf.size() - _pos;}
		inline size_t pos() {return _pos;}
		inline bool empty() {return _buf.empty();}
		inline void reset()
		{
			_pos = 0;
		}
		inline void clear()
		{
			_buf.clear();
			reset();
		}
		size_t first_of(byte b)
		{
			size_t c = _buf.size();
			for(size_t i = 0; i < c; ++ i)
			{
				if(_buf[i] == b)
				{
					return i;
				}
			}
			return (size_t)-1;
		}
		inline void pop_front(size_t n)
		{
			if(n >= _buf.size())
				_buf.clear();
			else
				_buf.erase(_buf.begin(), _buf.begin() + n);
		}
		inline void remove(size_t s, size_t n)
		{
			if(s >= _buf.size())
				return;
			if(s + n > _buf.size())
				n = _buf.size() - s;
			_buf.erase(_buf.begin() + s, _buf.begin() + s + n);
		}
		inline void append_zero(size_t len)
		{
			if(len > 0)
				_buf.resize(_buf.size() + len, 0);
		}
		inline void append(const byte * buf, size_t len)
		{
			_buf.insert(_buf.end(), buf, buf + len);
		}
		inline void prepend(const byte * buf, size_t len)
		{
			_buf.insert(_buf.begin(), buf, buf + len);
		}
		inline int read(byte * buf, size_t len)
		{
			if(_pos + len > _buf.size())
				len = _buf.size() - _pos;
			if(len == 0)
				return 0;
			memcpy(buf, &_buf[_pos], len);
			_pos += len;
			return (int)len;
		}
		template<typename T>
		Stream& operator>>(T& v)
		{
			read((byte *)&v, sizeof(T));
			return *this;
		}
		template<typename T>
		Stream& operator<<(const T& v)
		{
			append((byte *)&v, sizeof(T));
			return *this;
		}
		Stream& operator>>(std::vector<byte>& v)
		{
			read(&v.front(), v.size());
			return *this;
		}
		Stream& operator<<(const std::vector<byte>& v)
		{
			_buf.insert(_buf.end(), v.begin(), v.end());
			return *this;
		}
		template<typename T>
		Stream& operator>>(std::vector<T>& v)
		{
			uint n;
			v.clear();
			(*this)>>n;
			if(n > 0)
			{
				v.resize(n);
				for(uint i = 0; i < n; ++ i)
					read(&v[i], sizeof(T));
			}
			return *this;
		}
		template<typename T>
		Stream& operator<<(const std::vector<T>& v)
		{
			uint n = v.size();
			(*this)<<n;
			for(uint i = 0; i < n ; ++ i)
			{
				append((byte *)&v[i], sizeof(T));
			}
			return *this;
		}
		Stream& operator>>(std::string& v)
		{
			size_t s = _buf.size();
			v.clear();
			size_t n = _pos;
			while(n < s && _buf[n] != 0)
			{
				++ n;
			}
			if(n > _pos)
			{
				v.resize(n - _pos);
				memcpy(&v[0], &_buf[_pos], n - _pos);
			}
			if(n == s)
				_pos = n;
			else
				_pos = n + 1;
			return *this;
		}
		Stream& operator<<(const std::string& v)
		{
			_buf.insert(_buf.end(), v.begin(), v.end());
			_buf.push_back((byte)0);
			return *this;
		}
		Stream& operator<<(const char * v)
		{
			_buf.insert(_buf.end(), v, v + _STR.strlen(v) + 1);
			return *this;
		}
		Stream& operator+=(uint n)
		{
			_pos += n;
			if(_pos > _buf.size())
				_pos = _buf.size();
			return *this;
		}
		Stream& operator-=(uint n)
		{
			if(_pos < n)
				_pos = 0;
			else
				_pos -= n;
			return *this;
		}
	protected:
		size_t _pos;
		std::vector<byte> _buf;
	};
}

#endif // _STREAM_H_
