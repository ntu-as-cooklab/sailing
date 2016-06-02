#ifndef DATE_HPP
#define DATE_HPP

#include <iostream> // foward declare iostream

struct Date
{
	int year, month, day, hour;

	//Date(int year, int month, int day, int hour) : year(year), month(month), day(day), hour(hour) {}

	inline int leap(int y) { return (!(y%400))?1 : (!(y%100))?0 : (!(y%4))?1 : 0; }
	inline int days(int m, int y) { return m==2 ? 28+leap(y) : (m<8 ? m&1:!(m&1)) ? 31 : 30; }

	inline Date& operator++()
	{
		if (++hour >= 24)
			if(hour = 0, ++day >= days(month, year))
				if(day = 1, ++month > 12)
					month = 1, ++year;
		return *this;
	}
	inline Date operator++(int)
	{
		Date tmp(*this); // copy
		operator++(); // pre-increment
		return tmp;   // return old value
	}
	inline Date& operator--()
	{
		if (--hour < 0)
			if(hour = 23, --day < 1)
				if(day = days(month, year), month-- < 1)
					month = 12, --year;
		return *this;
	}
	inline Date operator--(int)
	{
		Date tmp(*this); // copy
		operator--(); // pre-increment
		return tmp;   // return old value
	}

	//inline friend int operator-(const Date& lhs, const Date& rhs)
  	//{
    //	return (lhs.year-rhs.year)*(365+leap())
  	//}

	inline friend bool operator< (const Date& lhs, const Date& rhs)
		{ return
			lhs.year != rhs.year ? lhs.year < rhs.year :
			lhs.month != rhs.month ? lhs.month < rhs.month :
			lhs.day != rhs.day ? lhs.day < rhs.day :
			lhs.hour != rhs.hour ? lhs.hour < rhs.hour :
			false;
		}
	inline friend bool operator> (const Date& lhs, const Date& rhs){ return rhs < lhs; }
	inline friend bool operator<=(const Date& lhs, const Date& rhs){ return !(lhs > rhs); }
	inline friend bool operator>=(const Date& lhs, const Date& rhs){ return !(lhs < rhs); }

	inline friend bool operator==(const Date& lhs, const Date& rhs)
		{ return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day && lhs.hour == rhs.hour; }
	inline friend bool operator!=(const Date& lhs, const Date& rhs){ return !(lhs == rhs); }

	inline friend std::ostream& operator<<(std::ostream& os, const Date& date)
		{ return os << date.year << "/" << date.month << "/" << date.day << " " << date.hour << "hr"; }

	inline friend std::istream& operator>>(std::istream& is, Date& date)
		{ return is >> date.year >> date.month >> date.day >> date.hour; }
};

#endif
