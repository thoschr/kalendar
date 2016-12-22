#ifndef DATEUTILTEST_H
#define DATEUTILTEST_H

#include "../test.h"
#include "../../util/dateutil.h"

class DateUtilTest
{
private:
    Date *mar_10_2016;
    Date *sep_28_1927;
    Date *feb_29_2016;
    Date *jan_1_2015;
    Date *dec_31_2014;
    Date *may_1_2102;
    Date *mar_1_2015;
    Date *mar_1_2016;
    Date *mar_1_2017;
    Date *mar_1_2018;
    Date *sep_1_1927;
    Date *jan_12_2015;
    Date *feb_21_2016;
    Date *feb_1_2016;
    Date *dec_14_2014;
    Date *jan_31_2015;
    Date *aug_20_2000;
    Date *aug_21_2000;

public:
    DateUtilTest();
    ~DateUtilTest();
    void test_all();
    void test_get_literal_month();
    void test_numeric2literal_day_of_week();
    void test_literal2numeric_day_of_week();
    void test_get_days_in_month();
    void test_date_from_timestamp();
    void test_get_first_day_of_month();
    void test_get_last_day_of_month();
    void test_increase_month();
    void test_decrease_month();
    void test_increase_day();
    void test_decrease_day();
    void test_increase_year();
    void test_decrease_year();
    void test_is_leap();
};

#endif
