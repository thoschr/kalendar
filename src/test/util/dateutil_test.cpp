#include "dateutil_test.h"

#include <QDebug>

DateUtilTest::DateUtilTest()
{
    this->march_2016 = new Date(10, 4, 3, 2016);
    this->may_2102 = new Date(1, 1, 5, 2102);
    this->sep_1927 = new Date(28, 3, 9, 1927);
    this->firstday_of_year_2015 = new Date(1, 4, 1, 2015);
    this->lastday_of_year_2014 = new Date(31, 3, 12, 2014);
    this->leap_year_2016 = new Date(29, 1, 2, 2016);
    this->first_march_2016 = new Date(1, 2, 3, 2016);
    this->first_sep_1927 = new Date(1, 4, 9, 1927);
    this->jan_2015 = new Date(12, 1, 1, 2015);
    this->lastday_jan_2015 = new Date(31, 6, 1, 2015);
    this->feb_2016 = new Date(21, 7, 2, 2016);
    this->first_feb_2016 = new Date(1, 1, 2, 2016);
    this->dec_2014 = new Date(14, 7, 12, 2014);
}

DateUtilTest::~DateUtilTest()
{
    delete this->march_2016;
    delete this->may_2102;
    delete this->sep_1927;
    delete this->firstday_of_year_2015;
    delete this->lastday_of_year_2014;
    delete this->leap_year_2016;
    delete this->first_march_2016;
    delete this->first_sep_1927;
    delete this->jan_2015;
    delete this->feb_2016;
    delete this->first_feb_2016;
    delete this->dec_2014;
}

void DateUtilTest::test_all() {
    test_get_days_in_month();
    test_literal2numeric_day_of_week();
    test_numeric2literal_day_of_week();
    test_get_literal_month();
    test_date_from_timestamp();
    test_get_first_day_of_month();
    test_get_last_day_of_month();
    test_increase_month();
    test_decrease_month();
}

void DateUtilTest::test_get_days_in_month() {
    Test::print("test_get_days_in_month ");
    ASSERT ((DateUtil::get_days_in_month(this->leap_year_2016->getMonth(), this->leap_year_2016->getYear()) == 29) &&
            (DateUtil::get_days_in_month(this->firstday_of_year_2015->getMonth(), this->firstday_of_year_2015->getYear()) == 31) && //January
            (DateUtil::get_days_in_month(this->sep_1927->getMonth(), this->sep_1927->getYear()) == 30))
}

void DateUtilTest::test_literal2numeric_day_of_week() {
    Test::print("test_literal2numeric_day_of_week ");
    ASSERT ((DateUtil::literal2numeric_day_of_week(string("Monday")) == 1) &&
            (DateUtil::literal2numeric_day_of_week(string("Sunday")) == 7) &&
            (DateUtil::literal2numeric_day_of_week(string("Thursday")) == 4) &&
            (DateUtil::literal2numeric_day_of_week(string("")) == -1))
}

void DateUtilTest::test_numeric2literal_day_of_week() {
    Test::print("test_numeric2literal_day_of_week ");
    ASSERT ((DateUtil::numeric2literal_day_of_week(1) == string("Monday")) &&
            (DateUtil::numeric2literal_day_of_week(7) == string("Sunday")) &&
            (DateUtil::numeric2literal_day_of_week(2) == string("Tuesday")) &&
            (DateUtil::numeric2literal_day_of_week(0) == string("")) &&
            (DateUtil::numeric2literal_day_of_week(8) == string("")))
}

void DateUtilTest::test_get_literal_month() {
    Test::print("test_timeutil_get_literal_month ");
    ASSERT ((DateUtil::get_literal_month(1) == string("January")) &&
            (DateUtil::get_literal_month(12) == string("December")) &&
            (DateUtil::get_literal_month(8) == string("August")) &&
            (DateUtil::get_literal_month(0) == string("")) &&
            (DateUtil::get_literal_month(13) == string("")))
}

void DateUtilTest::test_get_first_day_of_month() {
    Test::print("test_get_first_day_of_month ");
    ASSERT ((DateUtil::get_first_day_of_month(*this->firstday_of_year_2015) == *this->firstday_of_year_2015) &&
            (DateUtil::get_first_day_of_month(*this->march_2016) == *this->first_march_2016) &&
            (DateUtil::get_first_day_of_month(*this->sep_1927) == *this->first_sep_1927) &&
            (DateUtil::get_first_day_of_month(*this->jan_2015)) == *this->firstday_of_year_2015 &&
            (DateUtil::get_first_day_of_month(*this->feb_2016)) == *this->first_feb_2016)
}

void DateUtilTest::test_date_from_timestamp() {
    Test::print("test_date_from_timestamp ");
    ASSERT ((DateUtil::date_from_timestamp(1420070400)) == *this->firstday_of_year_2015 &&
            (DateUtil::date_from_timestamp(1419984000)) == *this->lastday_of_year_2014 &&
            (DateUtil::date_from_timestamp(1456012800)) == *this->feb_2016 &&
            (DateUtil::date_from_timestamp(4175884800)) == *this->may_2102 &&
            /* remember: the timestamp will be converted into an unsigned long */
            (DateUtil::date_from_timestamp(-1333670400)) == *this->sep_1927)
}

void DateUtilTest::test_get_last_day_of_month() {
    Test::print("test_get_last_day_of_month ");
    ASSERT ((DateUtil::get_last_day_of_month(*this->feb_2016)) == *this->leap_year_2016 &&
            (DateUtil::get_last_day_of_month(*this->dec_2014)) == *this->lastday_of_year_2014)
}

void DateUtilTest::test_increase_month() {
    Test::print("test_increase_month ");
    ASSERT (DateUtil::increase_month(*this->first_feb_2016) == *this->first_march_2016 &&
            DateUtil::increase_month(*this->lastday_of_year_2014) == *this->lastday_jan_2015)
}

void DateUtilTest::test_decrease_month() {
    Test::print("test_decrease_month ");
    ASSERT (DateUtil::decrease_month(*this->first_march_2016) == *this->first_feb_2016 &&
            DateUtil::decrease_month(*this->lastday_jan_2015) == *this->lastday_of_year_2014)
}
