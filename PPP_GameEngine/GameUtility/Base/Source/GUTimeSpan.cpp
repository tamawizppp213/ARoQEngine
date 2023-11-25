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
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	template <typename ... Args>
	std::string Format(const std::string& fmt, Args ... args)
	{
		size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args ...);
		std::vector<char> buf(len + 1);
		std::snprintf(&buf[0], len + 1, fmt.c_str(), args ...);
		return std::string(&buf[0], &buf[0] + len);
	}
}

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

TimeSpan TimeSpan::CreateFromSeconds(const double seconds)
{
	return TimeSpan((int64)floor(seconds * TICKS_PER_SECOND + 0.5));
}

TimeSpan TimeSpan::CreateFromMinutes(const double minutes)
{
	return TimeSpan((int64)floor(minutes * TICKS_PER_MINUTE + 0.5));
}
TimeSpan TimeSpan::CreateFromHours(const double hours)
{
	return TimeSpan((int64)floor(hours * TICKS_PER_HOUR + 0.5));
}
TimeSpan TimeSpan::CreateFromDays(const double days)
{
	return TimeSpan((int64)floor(days * TICKS_PER_DAY + 0.5));
}

gu::string TimeSpan::ToString() const 
{
	return GetDays() == 0 ? ToString("%h:%m:%s.%f") : ToString("%d.%h:%m:%s.%f");
}

gu::string TimeSpan::ToString(const gu::char8* format) const
{
	gu::string result;
	
	result += (_ticks < 0) ? "-" : "+"; // ‰ß‹Ž‚©–¢—ˆ‚©

	while (*format != ('\0'))
	{
		if ((*format == '%') && (*++format != '\0'))
		{
			switch (*format)
			{
				case 'd': result += Format("%i", GetDays()); break;
				case 'D': result += Format("%08i", GetDays()); break;
				case 'h': result += Format("%02i", GetHours());break;
				case 'm': result += Format("%02i", GetMinutes()); break;
				case 's': result += Format("%02i", GetSeconds()); break;
				case 'f': result += Format("%03i", GetFractionMillisecond()); break;
				case 'u': result += Format("%06i", GetFractionMicrosecond()); break;
				case 't': result += Format("%07i", GetFractionTicks()); break;
				case 'n': result += Format("%09i", GetFractionNanosecond()); break;
				default:
					result += *format;
			}
		}
		else
		{
			result += *format;
		}

		++format;
	}

	return result;
}
#pragma endregion Main Function