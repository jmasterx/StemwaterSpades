#ifndef CGE_TIME_OF_DAY_HPP
#define CGE_TIME_OF_DAY_HPP
#include <stdlib.h>
#include <string>
#include <sstream>
namespace cge
{
	class TimeOfDay
	{
	public:
		static int getSecond();
		static int getMinute();
		static int getHour();
		static int getMonth();
		static int getDay();
		static int getMonthDay();
		static int getYear();
		static std::string getShortTime();
		static std::string getDate();
	};
}

#endif
