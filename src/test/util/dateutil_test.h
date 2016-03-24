#ifndef DATEUTILTEST_H
#define DATEUTILTEST_H

#include "../test.h"
#include "../../util/dateutil.h"

class DateUtilTest
{
private:
    Date *march_2016;
    Date *sep_1927;
    Date *leap_year_2016;
    Date *firstday_of_year_2015;
    Date *lastday_of_year_2014;
    Date *may_2102;
    Date *first_march_2016;
    Date *first_sep_1927;
    Date *jan_2015;
    Date *feb_2016;
    Date *first_feb_2016;
public:
    DateUtilTest();
    ~DateUtilTest();
    void test_all();
    void test_get_literal_month();
    void test_numeric2literal_day_of_week();
    void test_literal2numeric_day_of_week();
    void test_get_days_in_month();
    void test_time_from_timestamp();
    void test_get_first_day_of_month();
    void test_get_last_day_of_month();
    void test_increase_month();
    void test_decrease_month();
};

#endif
