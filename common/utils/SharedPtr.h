#ifndef _SharedPtr_INCLUDED
#define _SharedPtr_INCLUDED


#include "AtomicCounter.h"
#include <algorithm>


namespace Utils {


class ReferenceCounter
	/// Simple ReferenceCounter object, does not delete itself when count reaches 0.
{
public:
	ReferenceCounter(): _cnt(1)
	{
	}

	void duplicate()
	{
		++_cnt;
	}

	int release()
	{
		return --_cnt;
	}
	
	int referenceCount() const
	{
		return _cnt.value();
	}

private:
	AtomicCounter _cnt;
};


template <class C>
class ReleasePolicy
	/// The default release policy for SharedPtr, which
	/// simply uses the delete operator to delete an object.
{
public:
	static void release(C* pObj)
		/// Delete the object.
		/// Note that pObj can be 0.
	{
		delete pObj;
	}
};


template <class C, class RC = ReferenceCounter, class RP = ReleasePolicy<C> >
class SharedPtr
	/// SharedPtr is a "smart" pointer for classes implementing
	/// reference counting based garbage collection.
	/// SharedPtr is thus similar to AutoPtr. Unlike the
	/// AutoPtr template, which can only be used with
	/// classes that support reference counting, SharedPtr
	/// can be used with any class. For this to work, a
	/// SharedPtr manages a reference count for the object
	/// it manages.
	/// 
	/// SharedPtr works in the following way:
	/// If an SharedPtr is assigned an ordinary pointer to
	/// an object (via the constructor or the assignment operator),
	/// it takes ownership of the object and the object's reference 
	/// count is initialized to one.
	/// If the SharedPtr is assigned another SharedPtr, the
	/// object's reference count is incremented by one.
	/// The destructor of SharedPtr decrements the object's
	/// reference count by one and deletes the object if the
	/// reference count reaches zero.
	/// SharedPtr supports dereferencing with both the ->
	/// and the * operator. An attempt to dereference a null
	/// SharedPtr results in a NullPointerException being thrown.
	/// SharedPtr also implements all relational operators and
	/// a cast operator in case dynamic casting of the encapsulated data types
	/// is required.
{
public:
	SharedPtr(): _pCounter(NULL), _ptr(0)
	{
	}

	SharedPtr(C* ptr): _pCounter(ptr ? (new RC) : NULL), _ptr(ptr)
	{
	}

	template <class Other, class OtherRP> 
	SharedPtr(const SharedPtr<Other, RC, OtherRP>& ptr): _pCounter(ptr._pCounter), _ptr(const_cast<Other*>(ptr.get()))
	{
		if(_pCounter)
			_pCounter->duplicate();
	}

	SharedPtr(const SharedPtr& ptr): _pCounter(ptr._pCounter), _ptr(ptr._ptr)
	{
		if(_pCounter)
			_pCounter->duplicate();
	}

	~SharedPtr()
	{
		release();
	}

	SharedPtr& assign(C* ptr)
	{
		if (get() != ptr)
		{
			RC* pTmp = ptr ? (new RC) : NULL;
			release();
			_pCounter = pTmp;
			_ptr = ptr;
		}
		return *this;
	}
	
	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}
	
	template <class Other, class OtherRP>
	SharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		if (ptr.get() != _ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	void reset()
	{
		assign(NULL);
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	template <class Other, class OtherRP>
	SharedPtr& operator = (const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(SharedPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
		std::swap(_pCounter, ptr._pCounter);
	}

	template <class Other> 
	SharedPtr<Other, RC, RP> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(_ptr);
		if (pOther)
			return SharedPtr<Other, RC, RP>(_pCounter, pOther);
		return SharedPtr<Other, RC, RP>();
	}

	template <class Other> 
	SharedPtr<Other, RC, RP> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(_ptr);
		return SharedPtr<Other, RC, RP>(_pCounter, pOther);
	}

	C* operator -> ()
	{
		return deref();
	}

	const C* operator -> () const
	{
		return deref();
	}

	C& operator * ()
	{
		return *deref();
	}

	const C& operator * () const
	{
		return *deref();
	}

	C* get()
	{
		return _ptr;
	}

	const C* get() const
	{
		return _ptr;
	}

	operator C* ()
	{
		return _ptr;
	}
	
	operator const C* () const
	{
		return _ptr;
	}

	bool operator ! () const
	{
		return _ptr == 0;
	}

	bool isNull() const
	{
		return _ptr == 0;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return get() == ptr.get();
	}

	bool operator == (const C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (C* ptr) const
	{
		return get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return get() != ptr.get();
	}

	bool operator != (const C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (C* ptr) const
	{
		return get() != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return get() < ptr.get();
	}

	bool operator < (const C* ptr) const
	{
		return get() < ptr;
	}

	bool operator < (C* ptr) const
	{
		return get() < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return get() <= ptr.get();
	}

	bool operator <= (const C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return get() > ptr.get();
	}

	bool operator > (const C* ptr) const
	{
		return get() > ptr;
	}

	bool operator > (C* ptr) const
	{
		return get() > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return get() >= ptr.get();
	}

	bool operator >= (const C* ptr) const
	{
		return get() >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return get() >= ptr;
	}
	
	int referenceCount() const
	{
		return _pCounter ? (_pCounter->referenceCount()) : 1;
	}

private:
	C* deref() const
	{
		if (!_ptr)
			throw std::bad_exception();

		return _ptr;
	}

	void release()
	{
		if(!_pCounter)
			return;
		int i = _pCounter->release();
		if (i == 0)
		{
			RP::release(_ptr);
			_ptr = 0;

			delete _pCounter;
			_pCounter = 0;
		}
	}

	SharedPtr(RC* pCounter, C* ptr): _pCounter(pCounter), _ptr(ptr)
		/// for cast operation
	{
		if(_pCounter)
			_pCounter->duplicate();
	}

private:
	RC* _pCounter;
	C*  _ptr;

	template <class OtherC, class OtherRC, class OtherRP> friend class SharedPtr;
};


template <class C, class RC, class RP>
inline void swap(SharedPtr<C, RC, RP>& p1, SharedPtr<C, RC, RP>& p2)
{
	p1.swap(p2);
}

template<class T>
class SharedClass
{
public:
	typedef SharedPtr<T> Pointer;
};

} // namespace Utils


#endif // SharedPtr_INCLUDED
