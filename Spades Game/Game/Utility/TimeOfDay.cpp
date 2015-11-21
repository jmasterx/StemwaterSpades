#include "Game/Utility/TimeOfDay.hpp"
#include <stdio.h>
#include <time.h>
#include <ctime>
namespace cge
{


	int TimeOfDay::getSecond()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_sec;
	}

	int TimeOfDay::getMinute()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_min;
	}

	int TimeOfDay::getHour()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_hour;
	}

	int TimeOfDay::getMonth()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_mon;
	}

	int TimeOfDay::getDay()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_wday;
	}

	int TimeOfDay::getMonthDay()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_mday;
	}

	int TimeOfDay::getYear()
	{
		time_t now = time(NULL);
		tm *tm_now = localtime(&now);
		return tm_now->tm_year + 1900;
	}

	std::string TimeOfDay::getShortTime()
	{
		std::stringstream ss;
		int hour = getHour();
		int minutes = getMinute();
	
		int properHour = hour;

		if(hour == 0)
		{
			properHour = 12;
		}
		else
		{
			properHour = hour % 12;
		}

		if(properHour < 10)
		{
			ss << "0";
		}
		ss << properHour;
		ss << ":";
		if(minutes < 10)
		{
			ss << "0";
		}

		ss << minutes << " ";

		std::string am = hour < 12 ? "AM" : "PM";
		ss << am;
		return ss.str();
	}

	std::string TimeOfDay::getDate()
	{
		std::stringstream s;
		s << getYear();
		s << "-";
		int month = getMonth();
		month++;
		if(month < 10)
			s << "0";
		s << month;
		s << "-";
		int day = getMonthDay();
		if(day < 10)
			s << "0";
		s << day;
		s << " ";
		s << getShortTime();
		return s.str();
	}

}

