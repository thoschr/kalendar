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
    Time *first_march_2016;
    Time *first_sep_1927;
public:
    TimeUtilTest();
    ~TimeUtilTest();
    void test_all();
    void test_get_literal_month();
    void test_numeric2literal_day_of_week();
    void test_literal2numeric_day_of_week();
    void test_get_days_in_month();
    void test_time_from_timestamp();
    void test_get_first_day_of_month();
    void test_increase_month();
    void test_decrease_month();
};

#endif // TIMEUTILTEST_H
