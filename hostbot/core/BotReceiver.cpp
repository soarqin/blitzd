#include "Config.h"
#include "BotReceiver.h"
#include "CmdHandler.h"
#include "BotCfg.h"
#include "Version.h"
#include "plugins/gametcp/PluginGameTcp.h"

namespace HostBot
{
	enum RECV_TYPE
	{
		RECV_NONE = 0,
		RECV_WC3 = 1,
		RECV_TELNETINIT = 2,
		RECV_TELNET = 3
	};
	BotReceiver::BotReceiver( int s ) : Network::TcpClient(s)
	{
		static uint sessionId = 0x10000;
		_session = sessionId ++;
		_type = RECV_NONE;
	}
	BotReceiver::~BotReceiver()
	{
	}
	void BotReceiver::OnRecv( byte * data, size_t len )
	{
		if(len == 0)
			return;
		if(_type == RECV_NONE)
		{
			if(data[0] == 0xF7)
				_type = RECV_WC3;
			else if(data[0] == '\r' || data[0] == '\n')
			{
				_type = RECV_TELNETINIT;
				char welcome[256];
				sprintf(welcome, "Welcome to visit console of Host Bot %d.%d.%d\r\nPassword: ", VER_MAJOR, VER_MINOR, VER_BUILD);
				Send(welcome);
				while(len > 0 && (*data == '\r' || *data == '\n'))
				{
					++ data;
					-- len;
				}
			}
			LOG_DEBUG(("Accepted new connection type for %08X: %d", (int)*this, _type));
		}
		_buffer.append(data, len);
		switch(_type)
		{
		case RECV_WC3:
			ProcessWC3();
			break;
		case RECV_TELNETINIT:
			switch(IdentifyTelnet())
			{
			case -1:
				Send("Invalid password");
				PendClose();
				break;
			case 1:
				Send("Correct password\r\n");
				TelnetNewline();
				_type = RECV_TELNET;
				break;
			}
			break;
		case RECV_TELNET:
			ProcessTelnet();
			break;
		}
	}

	void BotReceiver::Send( ushort op, Utils::Stream& buf )
	{
		std::vector<byte> sbuf;
		size_t len = buf.size();
		sbuf.resize(4 + len);
		*(ushort *)&sbuf[0] = op;
		*(ushort *)&sbuf[2] = 4 + (ushort)len;
		if(len > 0)
			memcpy(&sbuf[4], (byte *)buf, len);
		Network::TcpClient::Send(&sbuf[0], 4 + len);
	}

	void BotReceiver::Send( ushort op )
	{
		byte sbuf[4];
		*(ushort *)&sbuf[0] = op;
		*(ushort *)&sbuf[2] = 4;
		Network::TcpClient::Send(&sbuf[0], 4 );
	}

	void BotReceiver::Send( const char * str)
	{
		Network::TcpClient::Send((const byte *)str, _STR.strlen(str));
	}

	void BotReceiver::ProcessWC3()
	{
		size_t size;
		ushort psize;
		while((size = _buffer.size()) >= 4 && (psize = *(ushort *)&_buffer[2]) <= size)
		{
			const byte * buf = _buffer;
		    Utils::Stream st(buf + 4, psize - 4);
			Plugins::GameTcp::PluginGameTcp::GetSingleton().Process(*this, *(ushort *)buf, st);
			if(psize < size)
				_buffer.pop_front(psize);
			else
				_buffer.clear();
		}
	}

	void BotReceiver::TelnetNewline()
	{
		Network::TcpClient::Send((const byte *)"> ", 2);
	}

	bool BotReceiver::TelnetReadline(std::string& result)
	{
		size_t pos = 0;
		size_t len = _buffer.size();
		while(pos < len && ((byte*)_buffer)[pos] == 8)
		{
			++ pos;
		}
		if(pos > 0)
			_buffer.pop_front(pos);
		if(len > 1)
		{
			size_t len_ = len - 1;
			pos = 0;
			while(pos < len_)
			{
				if(_buffer[pos + 1] == 8)
				{
					_buffer.remove(pos, 2);
					if(len_ < 2)
						break;
					len_ -= 2;
				}
				else
					++ pos;
			}
		}
		pos = _buffer.first_of('\r');
		if(pos == (size_t)-1)
			return false;
		result.resize(pos);
		_buffer.read((byte *)&result[0], pos);
		while(pos < len && (((byte *)_buffer)[pos] == '\r' || ((byte *)_buffer)[pos] == '\n'))
		{
			++ pos;
		}
		if(pos < len)
			_buffer.pop_front(pos);
		else
			_buffer.clear();
		return true;
	}

	int BotReceiver::IdentifyTelnet()
	{
		std::string password;
		if(!TelnetReadline(password))
			return 0;
		if(password == botCfg.Password())
			return 1;
		return -1;
	}

	void BotReceiver::ProcessTelnet()
	{
		std::string line;
		if(!TelnetReadline(line))
			return;
		if(!line.empty())
		{
			if(!CmdHandler::GetSingleton().Process(*this, line))
			{
				Send("Bye-bye!\r\n");
				PendClose();
				return;
			}
		}
		TelnetNewline();
	}
}
