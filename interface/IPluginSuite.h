#ifndef _IPLUGINSUITE_H_
#define _IPLUGINSUITE_H_

#include "IPlugin.h"
#include "utils/Singleton.h"
#include "PluginManager.h"

#define DECLARE_PLUGIN_SUITE(name) \
class name: public Utils::Singleton<name>, public Interface::PluginManager { \
public: \
	name(); \
}; \

#define PLUGIN_SUITE_BEGIN(name) \
	name::name() { \

#define PLUGIN_ADD(p) \
	Load(Interface::IPlugin::Pointer(new p)); \

#define PLUGIN_SUITE_END() \
	} \

#endif // _IPLUGINSUITE_H_
