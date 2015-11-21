#include "Game/Database/DateTime.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

/**********************************************************************
* DateTimeDuration
***********************************************************************/

DateTimeDuration::DateTimeDuration(unsigned int _days, unsigned int _hours, unsigned int _minutes,
								   unsigned int _seconds, unsigned int _milliseconds, bool _is_negative)
{
	setDateTimeDuration(_days, _hours, _minutes, _seconds, _milliseconds, _is_negative);
}

DateTimeDuration::DateTimeDuration(const DateTimeDuration& dur)
{
	setDateTimeDuration(dur.days(), dur.hours(), dur.minutes(), dur.seconds(), dur.milliseconds(), dur.isNegative());
}

int DateTimeDuration::compare(const DateTimeDuration& rhs) const
{
	if (isZero() && rhs.isZero())
	{
		return 0;
	}

	if (days() == rhs.days())
	{
		if (hours() == rhs.hours())
		{
			if (minutes() == rhs.minutes())
			{
				if (seconds() == rhs.seconds())
				{
					if (milliseconds() == rhs.milliseconds())
					{
						return 0;
					}
					else if (milliseconds() < rhs.milliseconds())
					{
						return -1;
					}
					else
					{
						return 1;
					}
				}
				else if (seconds() < rhs.seconds())
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}
			else if (minutes() < rhs.minutes())
			{
				return -1;
			}
			else
			{
				return 1;
			}
		}
		else if (hours() < rhs.hours())
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	else if (days() < rhs.days())
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

DateTimeDuration& DateTimeDuration::operator=(const DateTimeDuration& rhs)
{
	setDateTimeDuration(rhs.days(), rhs.hours(), rhs.minutes(), rhs.seconds(), rhs.milliseconds(), rhs.isNegative());

	return *this;
}

bool DateTimeDuration::operator==(const DateTimeDuration& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val == 0)
	{
		return true;
	}

	return false;
}

bool DateTimeDuration::operator!=(const DateTimeDuration& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val != 0)
	{
		return true;
	}

	return false;
}

bool DateTimeDuration::operator<(const DateTimeDuration& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val < 0)
	{
		return true;
	}

	return false;
}

bool DateTimeDuration::operator<=(const DateTimeDuration& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val <= 0)
	{
		return true;
	}

	return false;
}

bool DateTimeDuration::operator>(const DateTimeDuration& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val > 0)
	{
		return true;
	}

	return false;
}

bool DateTimeDuration::operator>=(const DateTimeDuration& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val >= 0)
	{
		return true;
	}

	return false;
}

void DateTimeDuration::setDateTimeDuration(unsigned int _days, unsigned int _hours, unsigned int _minutes,
										   unsigned int _seconds, unsigned int _milliseconds, bool _is_negative)
{
	isNegative(_is_negative);
	days(_days);
	hours(_hours);
	minutes(_minutes);
	seconds(_seconds);
	milliseconds(_milliseconds);
}

bool DateTimeDuration::isZero() const
{
	if (days() == 0
		&& hours() == 0
		&& minutes() == 0
		&& seconds() == 0
		&& milliseconds() == 0)
	{
		return true;
	}

	return false;
}

unsigned int DateTimeDuration::days() const
{
	return __days;
}

unsigned int DateTimeDuration::days(unsigned int _days)
{      
	__days = _days;

	return __days;
}              

unsigned int DateTimeDuration::hours() const
{
	return __hours;
}

unsigned int DateTimeDuration::hours(unsigned int _hours)
{
	while (_hours >= 24)
	{
		unsigned int cur_days = days();
		days(++cur_days);
		_hours -= 24;
	}

	__hours = _hours;

	return __hours;
}

unsigned int DateTimeDuration::minutes() const
{
	return __minutes;
}

unsigned int DateTimeDuration::minutes(unsigned int _minutes)
{
	while (_minutes >= 60)
	{
		unsigned int cur_hours = hours();
		hours(++cur_hours);
		_minutes -= 60;
	}

	__minutes = _minutes;

	return __minutes;
}

unsigned int DateTimeDuration::seconds() const
{
	return __seconds;
}

unsigned int DateTimeDuration::seconds(unsigned int _seconds)
{
	while (_seconds >= 60)
	{
		unsigned int cur_mins = minutes();
		minutes(++cur_mins);
		_seconds -= 60;
	}

	__seconds = _seconds;

	return __seconds;
}

unsigned int DateTimeDuration::milliseconds() const
{
	return __milliseconds;
}

unsigned int DateTimeDuration::milliseconds(unsigned int _milliseconds)
{
	while (_milliseconds >= 1000)
	{
		unsigned int cur_secs = seconds();
		seconds(++cur_secs);
		_milliseconds -= 1000;
	}

	__milliseconds = _milliseconds;

	return __milliseconds;
}

bool DateTimeDuration::isNegative() const
{
	return __is_negative;
}

bool DateTimeDuration::isNegative(bool _is_negative)
{
	__is_negative = _is_negative;

	return __is_negative;
}

ostream& operator<<(ostream& os, const DateTimeDuration& dtd)
{
	if (dtd.isNegative())
	{
		os << "negative ";
	}

	os << dtd.days() << " days, " << dtd.hours() << " hours, " << dtd.minutes() << " minutes, "
		<< dtd.seconds() << " seconds, " << dtd.milliseconds() << " milliseconds";

	return os;
}

/**********************************************************************
* DateTime
***********************************************************************/

const unsigned short DateTime::MONTH_LENGTHS[] =
{31,28,31,30,31,30,31,31,30,31,30,31};

DateTime::DateTime(unsigned int _year, unsigned int _month, unsigned int _day, unsigned int _hour,
				   unsigned int _minute, unsigned int _second, unsigned int _millisecond)
{
	setDateTime(_year, _month, _day, _hour, _minute, _second, _millisecond);
}

DateTime::DateTime(const DateTime& dt)
{
	setDateTime(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.millisecond());
}

DateTime::DateTime(const tm& time_struct)
{
	setDateTime(time_struct.tm_year + 1900, time_struct.tm_mon + 1, time_struct.tm_mday, time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec, 0);
}

DateTime::DateTime(const time_t& time)
{
	tm* time_struct = localtime(&time);

	setDateTime(time_struct->tm_year + 1900, time_struct->tm_mon + 1, time_struct->tm_mday, time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec, 0);
}

int DateTime::compare(const DateTime& rhs) const
{
	if (year() == rhs.year())
	{      
		if (month() == rhs.month())
		{
			if (day() == rhs.day())
			{
				if (hour() == rhs.hour())
				{
					if (minute() == rhs.minute())
					{
						if (second() == rhs.second())
						{
							if (millisecond() == rhs.millisecond())
							{
								return 0;
							}

							else if (millisecond() < rhs.millisecond())
							{
								return -1;
							}

							else
							{
								return 1;
							}
						}

						else if (second() < rhs.second())
						{
							return -1;
						}

						else
						{
							return 1;
						}
					}

					else if (minute() < rhs.minute())
					{
						return -1;
					}

					else
					{
						return 1;
					}
				}

				else if (hour() < rhs.hour())
				{
					return -1;
				}

				else
				{
					return 1;
				}
			}

			else if (day() < rhs.day())
			{
				return -1;
			}

			else
			{
				return 1;
			}
		}

		else if (month() < rhs.month())
		{
			return -1;
		}

		else
		{
			return 1;
		}
	}

	else if (year() < rhs.year())
	{
		return -1;
	}

	else
	{
		return 1;
	}
}

DateTime& DateTime::operator=(const DateTime& rhs)
{
	setDateTime(rhs.year(), rhs.month(), rhs.day(), rhs.hour(), rhs.minute(), rhs.second(), rhs.millisecond());

	return *this;
}

bool DateTime::operator==(const DateTime& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val == 0)
	{
		return true;
	}

	return false;
}

bool DateTime::operator!=(const DateTime& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val != 0)
	{
		return true;
	}

	return false;
}

bool DateTime::operator<(const DateTime& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val < 0)
	{
		return true;
	}

	return false;
}

bool DateTime::operator<=(const DateTime& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val <= 0)
	{
		return true;
	}

	return false;
}

bool DateTime::operator>(const DateTime& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val > 0)
	{
		return true;
	}

	return false;
}

bool DateTime::operator>=(const DateTime& rhs) const
{
	int compare_val = compare(rhs);

	if (compare_val >= 0)
	{
		return true;
	}

	return false;
}

void DateTime::setDate(unsigned int _year, unsigned int _month, unsigned int _day)
{
	if (!validDate(_year, _month, _day))
	{
		throw InvalidDateTimeException(_year, _month, _day, hour(), minute(), second(), millisecond());
	}

	__year = _year;
	__month = _month;
	__day = _day;
}

void DateTime::setTime(unsigned int _hour, unsigned int _minute, unsigned int _second, unsigned int _millisecond)
{
	if (_hour >= 24 || _minute >= 60 || _second >= 60 || _millisecond >= 1000)
	{
		throw InvalidDateTimeException(year(), month(), day(), _hour, _minute, _second, _millisecond);
	}

	__hour = _hour;
	__minute = _minute;
	__second = _second;
	__millisecond = _millisecond;
}

void DateTime::setDateTime(unsigned int _year, unsigned int _month, unsigned int _day, unsigned int _hour, unsigned int _minute, unsigned int _second, unsigned int _millisecond)
{
	if (!validDate(_year, _month, _day) || _hour >= 24 || _minute >= 60 || _second >= 60 || _millisecond >= 1000)
	{
		throw InvalidDateTimeException(_year, _month, _day, _hour, _minute, _second, _millisecond);
	}

	__year = _year;
	__month = _month;
	__day = _day;
	__hour = _hour;
	__minute = _minute;
	__second = _second;
	__millisecond = _millisecond;  
}

unsigned int DateTime::year() const
{
	return __year;
}

unsigned int DateTime::year(unsigned int _year)
{
	if (_year == 0)
	{
		throw InvalidDateTimeException(_year, month(), day(), hour(), minute(), second(), millisecond());      
	}
	__year = _year;

	if (!validDate(_year, month(), day()))
	{
		__month = DEFAULT_MONTH;
		__day = DEFAULT_DAY;
	}

	return __year;
}

unsigned int DateTime::month() const
{
	return __month;
}

unsigned int DateTime::month(unsigned int _month)
{
	if (_month == 0 || _month > 12)
	{
		throw InvalidDateTimeException(year(), _month, day(), hour(), minute(), second(), millisecond());      
	}

	__month = _month;

	unsigned short days_in_month = daysInMonth(year(), _month);

	if (day() > days_in_month)
	{
		__day = DEFAULT_DAY;
	}

	return __month;
}

unsigned int DateTime::day() const
{
	return __day;
}

unsigned int DateTime::day(unsigned int _day)
{
	unsigned short days_in_month = daysInMonth(year(), month());

	if (_day == 0 || _day > days_in_month)
	{
		throw InvalidDateTimeException(year(), month(), _day, hour(), minute(), second(), millisecond());      
	}

	__day = _day;

	return __day;
}              

unsigned int DateTime::dayOfYear() const
{
	return getDayOfYear(year(), month(), day());
}

unsigned int DateTime::dayOfYear(unsigned int _dayOfYear)
{
	DateTime newDate = getReverseDayOfYear(year(), _dayOfYear);

	month(newDate.month());
	day(newDate.day());

	return _dayOfYear;
}

unsigned int DateTime::hour() const
{
	return __hour;
}

unsigned int DateTime::hour(unsigned int _hour)
{
	if (_hour >= 24)
	{
		throw InvalidDateTimeException(year(), month(), day(), _hour, minute(), second(), millisecond());      
	}

	__hour = _hour;

	return __hour;
}

unsigned int DateTime::minute() const
{
	return __minute;
}

unsigned int DateTime::minute(unsigned int _minute)
{
	if (_minute >= 60)
	{
		throw InvalidDateTimeException(year(), month(), day(), hour(), _minute, second(), millisecond());      
	}

	__minute = _minute;

	return __minute;
}

unsigned int DateTime::second() const
{
	return __second;
}

unsigned int DateTime::second(unsigned int _second)
{
	if (_second >= 60)
	{
		throw InvalidDateTimeException(year(), month(), day(), hour(), minute(), _second, millisecond());      
	}

	__second = _second;

	return __second;
}

unsigned int DateTime::millisecond() const
{
	return __millisecond;
}

unsigned int DateTime::millisecond(unsigned int _millisecond)
{
	if (_millisecond >= 1000)
	{
		throw InvalidDateTimeException(year(), month(), day(), hour(), minute(), second(), _millisecond);
	}

	__millisecond = _millisecond;

	return __millisecond;
}

DateTime DateTime::subtractDays(unsigned int _days) const
{
	DateTime tmp = *this;
	unsigned int _year = year();
	unsigned int _dayOfYear = dayOfYear();

	while (_days > 0)
	{
		if (_dayOfYear > _days)
		{
			_dayOfYear -= _days;
			_days = 0;
		}

		else
		{
			_days -= _dayOfYear;
			_year--;
			_dayOfYear = daysInYear(_year);
		}
	}

	tmp.year(_year);
	tmp.dayOfYear(_dayOfYear);

	return tmp;
}

DateTime DateTime::addDays(unsigned int _days) const
{
	DateTime tmp = *this;
	unsigned int _year = year();
	unsigned int _daysInYear = daysInYear(_year);
	unsigned int _dayOfYear = dayOfYear() + _days;

	while (_dayOfYear > _daysInYear)
	{
		_dayOfYear -= _daysInYear;
		_year++;
		_daysInYear = daysInYear(_year);
	}

	tmp.year(_year);
	tmp.dayOfYear(_dayOfYear);

	return tmp;
}

DateTime DateTime::subtractHours(unsigned int _hours) const
{
	DateTime tmp = *this;
	unsigned int currentH = hour();

	unsigned int subD = _hours / 24;
	unsigned int subH = _hours % 24;
	unsigned int newH;

	if (subH > currentH)
	{
		subD++;
		newH = 24 - (subH - currentH);
	}

	else
	{
		newH = currentH - subH;
	}

	if (subD > 0)
	{
		tmp = tmp.subtractDays(subD);
	}

	tmp.hour(newH);

	return tmp;
}

DateTime DateTime::addHours(unsigned int _hours) const
{
	DateTime tmp = *this;
	unsigned int currentH = hour();

	unsigned int addD = _hours / 24;
	unsigned int addH = _hours % 24;
	unsigned int newH;

	if ((addH + currentH) >= 24)
	{
		addD++;
		newH = (addH + currentH) % 24;
	}

	else
	{
		newH = addH + currentH;
	}

	if (addD > 0)
	{
		tmp = tmp.addDays(addD);
	}

	tmp.hour(newH);

	return tmp;
}

DateTime DateTime::subtractMinutes(unsigned int _minutes) const
{
	DateTime tmp = *this;
	unsigned int currentM = minute();

	unsigned int subH = _minutes / 60;
	unsigned int subM = _minutes % 60;
	unsigned int newM;

	if (subM > currentM)
	{
		subH++;
		newM = 60 - (subM - currentM);
	}

	else
	{
		newM = currentM - subM;
	}

	if (subH > 0)
	{
		tmp = tmp.subtractHours(subH);
	}

	tmp.minute(newM);

	return tmp;
}

DateTime DateTime::addMinutes(unsigned int _minutes) const
{
	DateTime tmp = *this;
	unsigned int currentM = minute();

	unsigned int addH = _minutes / 60;
	unsigned int addM = _minutes % 60;
	unsigned int newM;

	if ((addM + currentM) >= 60)
	{
		addH++;
		newM = (addM + currentM) % 60;
	}

	else
	{
		newM = addM + currentM;
	}

	if (addH > 0)
	{
		tmp = tmp.addHours(addH);
	}

	tmp.minute(newM);

	return tmp;
}

DateTime DateTime::subtractSeconds(unsigned int _seconds) const
{
	DateTime tmp = *this;
	unsigned int currentS = second();

	unsigned int subM = _seconds / 60;
	unsigned int subS = _seconds % 60;
	unsigned int newS;

	if (subS > currentS)
	{
		subM++;
		newS = 60 - (subS - currentS);
	}

	else
	{
		newS = currentS - subS;
	}

	if (subM > 0)
	{
		tmp = tmp.subtractMinutes(subM);
	}

	tmp.second(newS);

	return tmp;
}

DateTime DateTime::addSeconds(unsigned int _seconds) const
{
	DateTime tmp = *this;
	unsigned int currentS = second();

	unsigned int addM = _seconds / 60;
	unsigned int addS = _seconds % 60;
	unsigned int newS;

	if ((addS + currentS) >= 60)
	{
		addM++;
		newS = (addS + currentS) % 60;
	}

	else
	{
		newS = addS + currentS;
	}

	if (addM > 0)
	{
		tmp = tmp.addMinutes(addM);
	}

	tmp.second(newS);

	return tmp;
}

DateTime DateTime::subtractMilliseconds(unsigned int _milliseconds) const
{
	DateTime tmp = *this;
	unsigned int currentMs = millisecond();

	unsigned int subS = _milliseconds / 1000;
	unsigned int subMs = _milliseconds % 1000;
	unsigned int newMs;

	if (subMs > currentMs)
	{
		subS++;
		newMs = 1000 - (subMs - currentMs);
	}

	else
	{
		newMs = currentMs - subMs;
	}

	if (subS > 0)
	{
		tmp = tmp.subtractSeconds(subS);
	}

	tmp.millisecond(newMs);

	return tmp;
}

DateTime DateTime::addMilliseconds(unsigned int _milliseconds) const
{
	DateTime tmp = *this;
	unsigned int currentMs = millisecond();

	unsigned int addS = _milliseconds / 1000;
	unsigned int addMs = _milliseconds % 1000;
	unsigned int newMs;

	if ((addMs + currentMs) >= 1000)
	{
		addS++;
		newMs = (addMs + currentMs) % 1000;
	}

	else
	{
		newMs = addMs + currentMs;
	}

	if (addS > 0)
	{
		tmp = tmp.addSeconds(addS);
	}

	tmp.millisecond(newMs);

	return tmp;
}

DateTime DateTime::subtract(unsigned int _days, unsigned int _hours, unsigned int _minutes, unsigned int _seconds, unsigned int _milliseconds) const
{
	DateTime tmp = *this;

	tmp = tmp.subtractMilliseconds(_milliseconds);
	tmp = tmp.subtractSeconds(_seconds);
	tmp = tmp.subtractMinutes(_minutes);
	tmp = tmp.subtractHours(_hours);
	tmp = tmp.subtractDays(_days);

	return tmp;
}

DateTime DateTime::add(unsigned int _days, unsigned int _hours, unsigned int _minutes, unsigned int _seconds, unsigned int _milliseconds) const
{
	DateTime tmp = *this;

	tmp = tmp.addMilliseconds(_milliseconds);
	tmp = tmp.addSeconds(_seconds);
	tmp = tmp.addMinutes(_minutes);
	tmp = tmp.addHours(_hours);
	tmp = tmp.addDays(_days);

	return tmp;
}

DateTime DateTime::subtract(const DateTimeDuration& dur) const
{
	bool is_negative = !dur.isNegative();

	DateTimeDuration tmp = dur;
	tmp.isNegative(is_negative);

	return add(tmp);
}

DateTime DateTime::add(const DateTimeDuration& dur) const
{
	if (dur.isNegative())
	{
		return subtract(dur.days(), dur.hours(), dur.minutes(), dur.seconds(), dur.milliseconds());
	}
	else
	{
		return add(dur.days(), dur.hours(), dur.minutes(), dur.seconds(), dur.milliseconds());
	}
}

DateTimeDuration DateTime::timeBetween(const DateTime& rhs) const
{
	if (rhs == *this)
	{
		DateTimeDuration dur(0, 0, 0, 0, 0);

		return dur;
	}
	else if (rhs > *this)
	{
		DateTimeDuration dur = rhs.timeBetween(*this);
		dur.isNegative(true);

		return dur;
	}
	else
	{
		bool is_negative = false;

		static const unsigned long long int MS_PER_SEC = 1000;
		static const unsigned long long int MS_PER_MINUTE = MS_PER_SEC * 60;
		static const unsigned long long int MS_PER_HOUR = MS_PER_MINUTE * 60;
		static const unsigned long long int MS_PER_DAY = MS_PER_HOUR * 24;

		unsigned long long int ms = (hour() * MS_PER_HOUR) + (minute() * MS_PER_MINUTE) + (second() * MS_PER_SEC) + millisecond();
		unsigned long long int rhs_ms = (rhs.hour() * MS_PER_HOUR) + (rhs.minute() * MS_PER_MINUTE) + (rhs.second() * MS_PER_SEC) + rhs.millisecond();         

		DateTime stop_datetime;

		unsigned long long int total_ms = 0;

		//need to stop one day after in this case
		if (rhs_ms > ms)
		{
			stop_datetime = rhs.add(1, 0, 0, 0, 0);

			total_ms = MS_PER_DAY - (rhs_ms - ms);
		}
		else
		{
			stop_datetime = rhs;

			total_ms = ms - rhs_ms;
		}

		unsigned int hours = total_ms / MS_PER_HOUR;

		total_ms = total_ms % MS_PER_HOUR;

		unsigned int minutes = total_ms / MS_PER_MINUTE;

		total_ms = total_ms % MS_PER_MINUTE;

		unsigned int seconds = total_ms / MS_PER_SEC;

		total_ms = total_ms % MS_PER_SEC;

		unsigned int milliseconds = total_ms;

		unsigned int total_days = 0;

		unsigned int current_year = year();

		unsigned int current_day = dayOfYear();

		while (current_year > stop_datetime.year())
		{
			if (current_year == year())
			{
				total_days += dayOfYear();
			}
			else
			{
				total_days += daysInYear(current_year);
			}

			current_year--;
			current_day = dayOfYear();
		}

		while (current_day > stop_datetime.dayOfYear())
		{
			total_days++;
			current_day--;
		}

		DateTimeDuration dur(total_days, hours, minutes, seconds, milliseconds, is_negative);

		return dur;
	}
}

bool DateTime::isLeapYear(unsigned int _year)
{
	return (!(_year % 4) && (_year % 100) || !(_year % 400));
}

bool DateTime::validDate(unsigned int _year, unsigned int _month, unsigned int _day)
{
	if (_year == 0 || _month == 0 || _day == 0)
		return false;

	if (_month > 12)
		return false;

	unsigned short days_in_month = daysInMonth(_year, _month);

	if (_day > days_in_month)
		return false;

	return true;
}

unsigned short DateTime::daysInYear(unsigned int _year)
{              
	if (isLeapYear(_year))
	{
		return 366;
	}

	return 365;
}

unsigned short DateTime::daysInMonth(unsigned int _year, unsigned int _month)
{
	unsigned short days_in_month = MONTH_LENGTHS[_month - 1];

	if (_month == 2 && isLeapYear(_year))
	{
		days_in_month++;
	}

	return days_in_month;
}

unsigned short DateTime::getDayOfYear(unsigned int _year, unsigned int _month, unsigned int _day)
{
	unsigned int day_of_year = 0;

	for (unsigned int month = 1; month < _month; month++)
	{
		unsigned int num_days = DateTime::daysInMonth(_year, month);

		day_of_year += num_days;
	}

	day_of_year += _day;

	return day_of_year;
}

DateTime DateTime::getReverseDayOfYear(unsigned int _year, unsigned int _dayOfYear)
{
	unsigned int month;

	for (month = 1; month < 12; month++)
	{
		unsigned int num_days = DateTime::daysInMonth(_year, month);

		if (num_days >= _dayOfYear)
		{
			break;
		}

		_dayOfYear -= num_days;        
	}

	return DateTime(_year, month, _dayOfYear);
}

time_t DateTime::mktime() const
{
	tm time_struct;

	time_struct.tm_year = year() - 1900;
	time_struct.tm_mon = month() - 1;
	time_struct.tm_mday = day();
	time_struct.tm_hour = hour();
	time_struct.tm_min = minute();
	time_struct.tm_sec = second();

	time_t time = ::mktime(&time_struct);

	return time;
}

double DateTime::difftime(const DateTime& rhs) const
{
	time_t time_val = mktime();
	time_t rhs_time_val = rhs.mktime();

	double diff = ::difftime(time_val, rhs_time_val);

	return diff;
}

DateTime DateTime::now()
{
	time_t local_time = time(NULL);

	tm* time_struct = localtime(&local_time);

	return DateTime(*time_struct);
}

DateTime DateTime::now_utc()
{
	time_t utc_time = time(NULL);

	tm* time_struct = gmtime(&utc_time);

	return DateTime(*time_struct);
}

bool DateTime::setDateTimeFromSQL( const std::string& str )
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;

	if (sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) == 6)
	{
		setDateTime(year,month,day,hour,min,sec,0);
		return true;
	}

	return false;
}

std::string DateTime::dateToString() const
{
	std::stringstream ss;
	ss << month() << "/" << day() << "/" << year();
	return ss.str();
}

ostream& operator<<(ostream& os, const DateTime& ddt)
{
	os << setfill('0') << setw(2) << ddt.month() << "/" << setw(2) << ddt.day() << "/" << ddt.year()
		<< " " << setw(2) << ddt.hour() << ":" << setw(2) << ddt.minute() << ":"
		<<  setw(6) << fixed << setprecision(3) << static_cast<double>(static_cast<double>(ddt.second()) + (static_cast<double>(ddt.millisecond()) / 1000));

	return os;
}