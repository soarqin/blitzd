#include "Config.h"
#include "PreConfig.h"
#include "Timespan.h"
#include <algorithm>


namespace Utils {


const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS      = 1000*Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES      =   60*Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS        =   60*Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS         =   24*Timespan::HOURS;


Timespan::Timespan():
	_span(0)
{
}

	
Timespan::Timespan(TimeDiff microseconds):
	_span(microseconds)
{
}


Timespan::Timespan(long seconds, long microseconds):
	_span(TimeDiff(seconds)*SECONDS + microseconds)
{
}

	
Timespan::Timespan(int days, int hours, int minutes, int seconds, int microseconds):
	_span(TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS)
{
}


Timespan::Timespan(const Timespan& timespan):
	_span(timespan._span)
{
}


Timespan::~Timespan()
{
}


Timespan& Timespan::operator = (const Timespan& timespan)
{
	_span = timespan._span;
	return *this;
}


Timespan& Timespan::operator = (TimeDiff microseconds)
{
	_span = microseconds;
	return *this;
}


Timespan& Timespan::assign(int days, int hours, int minutes, int seconds, int microseconds)
{
	_span = TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS;
	return *this;
}


Timespan& Timespan::assign(long seconds, long microseconds)
{
	_span = TimeDiff(seconds)*SECONDS + TimeDiff(microseconds);
	return *this;
}


void Timespan::swap(Timespan& timespan)
{
	std::swap(_span, timespan._span);
}


Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(_span + d._span);
}


Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(_span - d._span);
}


Timespan& Timespan::operator += (const Timespan& d)
{
	_span += d._span;
	return *this;
}


Timespan& Timespan::operator -= (const Timespan& d)
{
	_span -= d._span;
	return *this;
}


Timespan Timespan::operator + (TimeDiff microseconds) const
{
	return Timespan(_span + microseconds);
}


Timespan Timespan::operator - (TimeDiff microseconds) const
{
	return Timespan(_span - microseconds);
}


Timespan& Timespan::operator += (TimeDiff microseconds)
{
	_span += microseconds;
	return *this;
}


Timespan& Timespan::operator -= (TimeDiff microseconds)
{
	_span -= microseconds;
	return *this;
}


} // namespace Utils
