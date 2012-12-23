#ifndef Event_WIN32_INCLUDED
#define Event_WIN32_INCLUDED


namespace Utils {


class EventImpl
{
protected:
	EventImpl(bool autoReset = false);		
	~EventImpl();
	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);
	void resetImpl();
	
private:
	HANDLE _event;
};


//
// inlines
//
inline void EventImpl::setImpl()
{
	if (!SetEvent(_event))
	{
		throw ("cannot signal event");
	}
}


inline void EventImpl::resetImpl()
{
	if (!ResetEvent(_event))
	{
		throw ("cannot reset event");
	}
}


} // namespace Utils


#endif // Event_WIN32_INCLUDED
