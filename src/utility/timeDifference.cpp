#include "utility/timeDifference.h"

#include <chrono>

TimeDifference::TimeDifference()
{
	this->startMs = TimeDifference::getCurrentTime();
	this->lastMs = this->startMs;
}

long TimeDifference::getCurrentTime()
{
	return duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long TimeDifference::getDifferenceWithStart() const
{
	return TimeDifference::getCurrentTime() - this->startMs;
}

long TimeDifference::getDifferenceWithLast() const
{
	return TimeDifference::getCurrentTime() - this->lastMs;
}

void TimeDifference::updateLast()
{
	this->lastMs = TimeDifference::getCurrentTime();
}