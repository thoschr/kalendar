#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <time.h>
#include <string>
#include <list>

#include "../model/date.h"

using namespace std;

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
    static Time get_first_day_of_month(Time &time);
    static Time get_last_day_of_month(Time &time);
    static Time increase_month(Time time);
    static Time decrease_month(Time time);
};

#endif // TIMEUTIL_H
