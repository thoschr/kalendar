#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <time.h>
#include <string>

using namespace std;

class Time {
private:
    int day;
    int month;
    int year;
public:
    Time(int day, int month, int year) {
        this->day = day;
        this->month = month;
        this->year = year;
    }
    int getDay() { return day; }
    int getMonth() { return month; }
    int getYear() { return year; }
    Time& operator=(const Time& t);
};

class TimeUtil
{
public:
    TimeUtil();
    Time get_current_time();
    string get_literal_day_of_week(int d); /* Monday => 1, Tuesday => 2, ... */
    int get_numeric_day_of_week(string d); /* 1 => Monday, 2 => Tuesday, ... */
};

#endif // TIMEUTIL_H
