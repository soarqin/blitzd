#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace Utils
{
	template<class T>
	class Singleton
	{
	public:
		inline static T& GetSingleton() {static T single; return single;}
		inline static T* GetSingletonPtr() {return &GetSingleton();}
	};
}

#endif // _SINGLETON_H_

