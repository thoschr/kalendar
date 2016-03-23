#include "timeutil_test.h"

TimeUtilTest::TimeUtilTest()
{
    this->march_2016 = new Time(10, 4, 3, 2016);
    this->may_2102 = new Time(1, 1, 5, 2102);
    this->sep_1927 = new Time(28, 3, 9, 1927);
    this->firstday_of_year_2015 = new Time(1, 4, 1, 2015);
    this->lastday_of_year_2014 = new Time(31, 3, 12, 2014);
    this->leap_year_2016 = new Time(29, 1, 2, 2016);
    this->first_march_2016 = new Time(1, 2, 3, 2016);
    this->first_sep_1927 = new Time(1, 4, 9, 1927);
    this->jan_2015 = new Time(12, 1, 1, 2015);
    this->feb_2016 = new Time(21, 7, 2, 2016);
    this->first_feb_2016 = new Time(1, 1, 2, 2016);
}

TimeUtilTest::~TimeUtilTest()
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
}

void TimeUtilTest::test_all() {
    test_get_days_in_month();
    test_literal2numeric_day_of_week();
    test_numeric2literal_day_of_week();
    test_get_literal_month();
    test_get_first_day_of_month();
    test_increase_month();
    test_decrease_month();
}

void TimeUtilTest::test_get_days_in_month() {
    Test::print("test_get_days_in_month ");
    ASSERT ((TimeUtil::get_days_in_month(this->leap_year_2016->getMonth(), this->leap_year_2016->getYear()) == 29) &&
            (TimeUtil::get_days_in_month(this->firstday_of_year_2015->getMonth(), this->firstday_of_year_2015->getYear()) == 31) && //January
            (TimeUtil::get_days_in_month(this->sep_1927->getMonth(), this->sep_1927->getYear()) == 30))
}

void TimeUtilTest::test_literal2numeric_day_of_week() {
    Test::print("test_literal2numeric_day_of_week ");
    ASSERT ((TimeUtil::literal2numeric_day_of_week(string("Monday")) == 1) &&
            (TimeUtil::literal2numeric_day_of_week(string("Sunday")) == 7) &&
            (TimeUtil::literal2numeric_day_of_week(string("Thursday")) == 4) &&
            (TimeUtil::literal2numeric_day_of_week(string("")) == -1))
}

void TimeUtilTest::test_numeric2literal_day_of_week() {
    Test::print("test_numeric2literal_day_of_week ");
    ASSERT ((TimeUtil::numeric2literal_day_of_week(1) == string("Monday")) &&
            (TimeUtil::numeric2literal_day_of_week(7) == string("Sunday")) &&
            (TimeUtil::numeric2literal_day_of_week(2) == string("Tuesday")) &&
            (TimeUtil::numeric2literal_day_of_week(0) == string("")) &&
            (TimeUtil::numeric2literal_day_of_week(8) == string("")))
}

void TimeUtilTest::test_get_literal_month() {
    Test::print("test_timeutil_get_literal_month ");
    ASSERT ((TimeUtil::get_literal_month(1) == string("January")) &&
            (TimeUtil::get_literal_month(12) == string("December")) &&
            (TimeUtil::get_literal_month(8) == string("August")) &&
            (TimeUtil::get_literal_month(0) == string("")) &&
            (TimeUtil::get_literal_month(13) == string("")))
}

void TimeUtilTest::test_get_first_day_of_month() {
    Test::print("test_get_first_day_of_month ");
    ASSERT ((TimeUtil::get_first_day_of_month(*this->firstday_of_year_2015) == *this->firstday_of_year_2015) &&
            (TimeUtil::get_first_day_of_month(*this->march_2016) == *this->first_march_2016) &&
            (TimeUtil::get_first_day_of_month(*this->sep_1927) == *this->first_sep_1927) &&
            (TimeUtil::get_first_day_of_month(*this->jan_2015)) == *this->firstday_of_year_2015 &&
            (TimeUtil::get_first_day_of_month(*this->feb_2016)) == *this->first_feb_2016)
}

void TimeUtilTest::test_get_last_day_of_month() {

}

void TimeUtilTest::test_increase_month() {
    Test::print("test_increase_month ");
}

void TimeUtilTest::test_decrease_month() {
    Test::print("test_decrease_month ");
}
