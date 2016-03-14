#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <time.h>
#include <string>
#include <list>

using namespace std;

class Time {
private:
    int mday; //month day
    int wday; //week day
    int month;
    int year;
public:
    Time(int mday, int wday, int month, int year) {
        this->mday = mday;
        this->wday = wday;
        this->month = month;
        this->year = year;
    }
    int getMonthDay() { return mday; }
    int getWeekDay() { return wday; }
    int getMonth() { return month; }
    int getYear() { return year; }
    Time& operator=(const Time& t);
};

class TimeUtil
{
public:
    TimeUtil();
    static Time get_current_time();
    static string get_literal_month(int m); /* 1 => January, ... */
    static string numeric2literal_day_of_week(int d); /* Monday => 1, Tuesday => 2, ... */
    static int literal2numeric_day_of_week(string d); /* 1 => Monday, 2 => Tuesday, ... */
    static int get_days_in_month(int month, int year);
    static Time time_from_timestamp(unsigned long timestamp);
    static list<int> get_special_days_in_month(int month, int year); /* TODO: implement this [priority: low] */
};

#endif // TIMEUTIL_H
