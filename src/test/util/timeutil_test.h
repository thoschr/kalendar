#ifndef TIMEUTILTEST_H
#define TIMEUTILTEST_H

#include "../test.h"
#include "../../util/timeutil.h"

class TimeUtilTest
{
private:
    Time *march_2016;
    Time *sep_1927;
    Time *leap_year;
    Time *firstday_of_year;
    Time *lastday_of_year;
    Time *may_2102;
public:
    TimeUtilTest();
    ~TimeUtilTest();
    void test_all();
    void test_get_literal_month();
    void test_numeric2literal_day_of_week();
    void test_literal2numeric_day_of_week();
    void test_get_days_in_month();
    void test_time_from_timestamp();
    void get_first_weekday_of_month();
};

#endif // TIMEUTILTEST_H
