#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "utils/Singleton.h"
#include "core/Client.h"

namespace Core
{
	namespace Command
	{
		class CommandHandler
		{
		public:
			virtual bool IsSingle() = 0;
			virtual void Process(Client&, std::string, std::string) = 0;
		};
		class Command:
			public Utils::Singleton<Command>
		{
		public:
			Command();
			void Process(Client&, std::string);
			void Register(std::string, CommandHandler *);
		private:
			std::map<std::string, CommandHandler *> _cmd_map;
		};
	}
}

#define COMMAND_HANDLER_BEGIN(n, b) \
	namespace Core \
	{ \
		namespace Command \
		{ \
			class n: public CommandHandler, \
				public Utils::Singleton<n> \
			{ \
			public: \
				virtual bool IsSingle() {return b;} \
				virtual void Process(Client& client, std::string arg1, std::string argr) \
				{ \

#define COMMAND_HANDLER_END() \
				} \
			}; \
		} \
	} \

#endif // _COMMAND_H_
