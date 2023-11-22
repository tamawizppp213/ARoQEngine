//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTimeSpan.cpp
///             @brief  temp
///             @author toide
///             @date   2023/11/22 0:05:17
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUTimeSpan.hpp"
#include "../Include/GUAssert.hpp"
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
TimeSpan::TimeSpan(const int64 ticks) : _ticks(ticks)
{
	Check((MIN_TICKS <= _ticks) && (_ticks <= MAX_TICKS));
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void TimeSpan::Assign(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano)
{
	int64 totalTicks = 0;

	totalTicks += days * TICKS_PER_DAY;
	totalTicks += hours * TICKS_PER_HOUR;
	totalTicks += minutes * TICKS_PER_MINUTE;
	totalTicks += seconds * TICKS_PER_SECOND;
	totalTicks += fractionNano / NANOSECONDS_PER_TICK;

	Check((MIN_TICKS <= totalTicks && totalTicks <= MAX_TICKS));

	_ticks = totalTicks;
}

TimeSpan TimeSpan::FromSeconds(const double seconds)
{
	return TimeSpan((int64)floor(seconds * TICKS_PER_SECOND + 0.5));
}

TimeSpan TimeSpan::FromMinutes(const double minutes)
{
	return TimeSpan((int64)floor(minutes * TICKS_PER_MINUTE + 0.5));
}
TimeSpan TimeSpan::FromHours(const double hours)
{
	return TimeSpan((int64)floor(hours * TICKS_PER_HOUR + 0.5));
}
TimeSpan TimeSpan::FromDays(const double days)
{
	return TimeSpan((int64)floor(days * TICKS_PER_DAY + 0.5));
}

#pragma endregion Main Function