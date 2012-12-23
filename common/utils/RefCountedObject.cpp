#include "Config.h"
#include "RefCountedObject.h"


namespace Utils {


RefCountedObject::RefCountedObject(): _counter(1)
{
}


RefCountedObject::~RefCountedObject()
{
}


} // namespace Utils
