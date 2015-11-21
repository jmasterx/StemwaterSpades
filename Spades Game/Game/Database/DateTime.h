#include <sstream>
#include <iostream>
#include <ctime>
#include <exception>

#ifndef DATETIME_H
#define DATETIME_H

class DateTimeDuration
{

public:
	static const unsigned int DEFAULT_DAYS = 0;
	static const unsigned int DEFAULT_HOURS = 0;
	static const unsigned int DEFAULT_MINS = 0;
	static const unsigned int DEFAULT_SECS = 0;
	static const unsigned int DEFAULT_MSECS = 0;
	static const bool DEFAULT_IS_NEGATIVE = false;

	DateTimeDuration(unsigned int _days = DEFAULT_DAYS, unsigned int _hours = DEFAULT_HOURS, unsigned int _minutes = DEFAULT_MINS, 
		unsigned int _seconds = DEFAULT_SECS, unsigned int _milliseconds = DEFAULT_MSECS, bool _is_negative = DEFAULT_IS_NEGATIVE);
	DateTimeDuration(const DateTimeDuration& dur);

	int compare(const DateTimeDuration& rhs) const;
	DateTimeDuration& operator=(const DateTimeDuration& rhs);
	bool operator==(const DateTimeDuration& rhs) const;
	bool operator!=(const DateTimeDuration& rhs) const;
	bool operator<(const DateTimeDuration& rhs) const;
	bool operator<=(const DateTimeDuration& rhs) const;
	bool operator>(const DateTimeDuration& rhs) const;
	bool operator>=(const DateTimeDuration& rhs) const;		
	void setDateTimeDuration(unsigned int _days, unsigned int _hours, unsigned int _minutes, 
		unsigned int _seconds, unsigned int _milliseconds, bool _is_negative);
	bool isZero() const;
	unsigned int days() const;
	unsigned int days(unsigned int _days);			
	unsigned int hours() const;
	unsigned int hours(unsigned int _hours);
	unsigned int minutes() const;
	unsigned int minutes(unsigned int _minutes);
	unsigned int seconds() const;
	unsigned int seconds(unsigned int _seconds);
	unsigned int milliseconds() const;
	unsigned int milliseconds(unsigned int _milliseconds);
	bool isNegative() const;
	bool isNegative(bool _is_negative);

private:
	unsigned int __days;
	unsigned int __hours;
	unsigned int __minutes;
	unsigned int __seconds;
	unsigned int __milliseconds;
	bool __is_negative;
};

std::ostream& operator<<(std::ostream& os, const DateTimeDuration& dtd);

class DateTime
{
public:
	static const unsigned int DEFAULT_YEAR = 1970;
	static const unsigned int DEFAULT_MONTH = 1;
	static const unsigned int DEFAULT_DAY = 1;
	static const unsigned int DEFAULT_HOUR = 0;
	static const unsigned int DEFAULT_MIN = 0;
	static const unsigned int DEFAULT_SEC = 0;
	static const unsigned int DEFAULT_MSEC = 0;

	static const unsigned short MONTH_LENGTHS[];

	DateTime(unsigned int _year = DEFAULT_YEAR, unsigned int _month = DEFAULT_MONTH, unsigned int _day = DEFAULT_DAY, unsigned int _hour = DEFAULT_HOUR,
		unsigned int _minute = DEFAULT_MIN, unsigned int _second = DEFAULT_SEC, unsigned int _millisecond = DEFAULT_MSEC);
	DateTime(const DateTime& dt);
	DateTime(const tm& time_struct);
	DateTime(const time_t& time);

	int compare(const DateTime& rhs) const;
	DateTime& operator=(const DateTime& rhs);
	bool operator==(const DateTime& rhs) const;
	bool operator!=(const DateTime& rhs) const;
	bool operator<(const DateTime& rhs) const;
	bool operator<=(const DateTime& rhs) const;
	bool operator>(const DateTime& rhs) const;
	bool operator>=(const DateTime& rhs) const;
	void setDate(unsigned int _year, unsigned int _month, unsigned int _day);
	void setTime(unsigned int _hour, unsigned int _minute, unsigned int _second, unsigned int _millisecond);
	void setDateTime(unsigned int _year, unsigned int _month, unsigned int _day, unsigned int _hour, unsigned int _minute, unsigned int _second, unsigned int _millisecond);
	bool setDateTimeFromSQL(const std::string& str);
	unsigned int year() const;
	unsigned int year(unsigned int _year);
	unsigned int month() const;
	unsigned int month(unsigned int _month);
	unsigned int day() const;
	unsigned int day(unsigned int _day);
	unsigned int dayOfYear() const;
	unsigned int dayOfYear(unsigned int _dayOfYear);			
	unsigned int hour() const;
	unsigned int hour(unsigned int _hour);
	unsigned int minute() const;
	unsigned int minute(unsigned int _minute);
	unsigned int second() const;
	unsigned int second(unsigned int _second);
	unsigned int millisecond() const;
	unsigned int millisecond(unsigned int _millisecond);
	DateTime subtractDays(unsigned int _days) const;
	DateTime addDays(unsigned int _days) const;
	DateTime subtractHours(unsigned int _hours) const;
	DateTime addHours(unsigned int _hours) const;	
	DateTime subtractMinutes(unsigned int _minutes) const;
	DateTime addMinutes(unsigned int _minutes) const;	
	DateTime subtractSeconds(unsigned int _seconds) const;
	DateTime addSeconds(unsigned int _seconds) const;	
	DateTime subtractMilliseconds(unsigned int _milliseconds) const;
	DateTime addMilliseconds(unsigned int _milliseconds) const;
	DateTime subtract(unsigned int _days = 0, unsigned int _hours = 0, unsigned int _minutes = 0, 
		unsigned int _seconds = 0, unsigned int _milliseconds = 0) const;
	DateTime add(unsigned int _days = 0, unsigned int _hours = 0, unsigned int _minutes = 0, 
		unsigned int _seconds = 0, unsigned int _milliseconds = 0) const;	
	DateTime subtract(const DateTimeDuration& dur) const;
	DateTime add(const DateTimeDuration& dur) const;
	DateTimeDuration timeBetween(const DateTime& rhs) const;
	static bool isLeapYear(unsigned int year);
	static bool validDate(unsigned int _year, unsigned int _month, unsigned int _day);
	static unsigned short daysInYear(unsigned int _year);
	static unsigned short daysInMonth(unsigned int _year, unsigned int _month);
	static unsigned short getDayOfYear(unsigned int _year, unsigned int _month, unsigned int _day);
	static DateTime getReverseDayOfYear(unsigned int _year, unsigned int _dayOfYear);
	time_t mktime() const;
	double difftime(const DateTime& rhs) const;
	static DateTime now();
	static DateTime now_utc();
	std::string dateToString() const;

	class InvalidDateTimeException: public std::exception
	{
	public:
		InvalidDateTimeException() throw() : std::exception()
		{
			errorString = "Invalid DateTime";
		}

		InvalidDateTimeException(unsigned int _year, unsigned int _month, unsigned int _day, unsigned int _hour,
			unsigned int _minute, unsigned int _second, unsigned int _millisecond) throw() : std::exception()
		{
			std::ostringstream oss;
			oss << "Invalid DateTime: year - " << _year << ", month - " << _month << ", day - " << _day
				<< ", hour - " << _hour << ", minute - " << _minute << ", second - " << _second << ", millisecond - " << _millisecond;

			errorString = oss.str();
		}

		InvalidDateTimeException(const InvalidDateTimeException& e) throw() : std::exception(e)
		{
			errorString = e.errorString;
		}

		InvalidDateTimeException(const std::string& _errorString) throw() : std::exception()
		{
			errorString = _errorString;
		}

		~InvalidDateTimeException() throw()
		{

		}

		virtual const char* what() const throw()
		{
			return errorString.c_str();
		}

		virtual std::string getError()
		{
			return errorString;
		}

	protected:
		std::string errorString;
	};		

private:
	unsigned int __year;
	unsigned int __month;
	unsigned int __day;
	unsigned int __hour;
	unsigned int __minute;
	unsigned int __second;
	unsigned int __millisecond;
};

std::ostream& operator<<(std::ostream& os, const DateTime& ddt);

#endif