#include "dateutil_test.h"

#include <QDebug>

DateUtilTest::DateUtilTest()
{
    this->mar_10_2016 = new Date(10, 4, 3, 2016);
    this->may_1_2102 = new Date(1, 1, 5, 2102);
    this->sep_28_1927 = new Date(28, 3, 9, 1927);
    this->jan_1_2015 = new Date(1, 4, 1, 2015);
    this->dec_31_2014 = new Date(31, 3, 12, 2014);
    this->feb_29_2016 = new Date(29, 1, 2, 2016);
    this->mar_1_2015 = new Date(1, 7, 3, 2015);
    this->mar_1_2016 = new Date(1, 2, 3, 2016);
    this->mar_1_2017 = new Date(1, 3, 3, 2017);
    this->mar_1_2018 = new Date(1, 4, 3, 2018);
    this->sep_1_1927 = new Date(1, 4, 9, 1927);
    this->jan_12_2015 = new Date(12, 1, 1, 2015);
    this->jan_31_2015 = new Date(31, 6, 1, 2015);
    this->feb_21_2016 = new Date(21, 7, 2, 2016);
    this->feb_1_2016 = new Date(1, 1, 2, 2016);
    this->dec_14_2014 = new Date(14, 7, 12, 2014);
    this->aug_20_2000 = new Date(20, 7, 8, 2000);
    this->aug_21_2000 = new Date(21, 1, 8, 2000);
}

DateUtilTest::~DateUtilTest()
{
    delete this->mar_10_2016;
    delete this->may_1_2102;
    delete this->sep_28_1927;
    delete this->jan_1_2015;
    delete this->dec_31_2014;
    delete this->feb_29_2016;
    delete this->mar_1_2015;
    delete this->mar_1_2016;
    delete this->mar_1_2017;
    delete this->mar_1_2018;
    delete this->sep_1_1927;
    delete this->jan_12_2015;
    delete this->feb_21_2016;
    delete this->feb_1_2016;
    delete this->dec_14_2014;
    delete this->aug_20_2000;
    delete this->aug_21_2000;
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
    test_increase_day();
    test_decrease_day();
    test_increase_year();
    test_decrease_year();
    test_is_leap();
}

void DateUtilTest::test_get_days_in_month() {
    Test::print("test_get_days_in_month ");
    ASSERT ((DateUtil::get_days_in_month(this->feb_29_2016->getMonth(), this->feb_29_2016->getYear()) == 29) &&
            (DateUtil::get_days_in_month(this->jan_1_2015->getMonth(), this->jan_1_2015->getYear()) == 31) && //January
            (DateUtil::get_days_in_month(this->sep_28_1927->getMonth(), this->sep_28_1927->getYear()) == 30))
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
    ASSERT ((DateUtil::get_first_day_of_month(*this->jan_1_2015) == *this->jan_1_2015) &&
            (DateUtil::get_first_day_of_month(*this->mar_10_2016) == *this->mar_1_2016) &&
            (DateUtil::get_first_day_of_month(*this->sep_28_1927) == *this->sep_1_1927) &&
            (DateUtil::get_first_day_of_month(*this->jan_12_2015)) == *this->jan_1_2015 &&
            (DateUtil::get_first_day_of_month(*this->feb_21_2016)) == *this->feb_1_2016)
}

void DateUtilTest::test_date_from_timestamp() {
    Test::print("test_date_from_timestamp ");
    ASSERT ((DateUtil::date_from_timestamp(1420070400)) == *this->jan_1_2015 &&
            (DateUtil::date_from_timestamp(1419984000)) == *this->dec_31_2014 &&
            (DateUtil::date_from_timestamp(1456012800)) == *this->feb_21_2016 &&
            (DateUtil::date_from_timestamp(4175884800)) == *this->may_1_2102 )
}

void DateUtilTest::test_get_last_day_of_month() {
    Test::print("test_get_last_day_of_month ");
    ASSERT ((DateUtil::get_last_day_of_month(*this->feb_21_2016)) == *this->feb_29_2016 &&
            (DateUtil::get_last_day_of_month(*this->dec_14_2014)) == *this->dec_31_2014)
}

void DateUtilTest::test_increase_month() {
    Test::print("test_increase_month ");
    ASSERT (DateUtil::increase_month(*this->feb_1_2016) == *this->mar_1_2016 &&
            DateUtil::increase_month(*this->dec_31_2014) == *this->jan_31_2015)
}

void DateUtilTest::test_decrease_month() {
    Test::print("test_decrease_month ");
    ASSERT (DateUtil::decrease_month(*this->mar_1_2016) == *this->feb_1_2016 &&
            DateUtil::decrease_month(*this->jan_31_2015) == *this->dec_31_2014)
}

void DateUtilTest::test_increase_day() {
    Test::print("test_increase_day ");
    ASSERT (DateUtil::increase_day(*this->feb_29_2016) == *this->mar_1_2016 &&
            DateUtil::increase_day(*this->dec_31_2014) == *this->jan_1_2015 &&
            DateUtil::increase_day(*this->aug_20_2000) == *this->aug_21_2000)
}

void DateUtilTest::test_decrease_day() {
    Test::print("test_decrease_day ");
    ASSERT (DateUtil::decrease_day(*this->mar_1_2016) == *this->feb_29_2016 &&
            DateUtil::decrease_day(*this->jan_1_2015) == *this->dec_31_2014 &&
            DateUtil::decrease_day(*this->aug_21_2000) == *this->aug_20_2000)
}

void DateUtilTest::test_increase_year() {
    Test::print("test_increase_year ");
    ASSERT (DateUtil::increase_year(*this->feb_29_2016) == *this->mar_1_2017 &&
            DateUtil::increase_year(*this->mar_1_2016) == *this->mar_1_2017 &&
            DateUtil::increase_year(*this->mar_1_2017) == *this->mar_1_2018 &&
            DateUtil::increase_year(*this->mar_1_2015) == *this->mar_1_2016)
}

void DateUtilTest::test_decrease_year() {
    Test::print("test_decrease_year ");
    ASSERT (DateUtil::decrease_year(*this->mar_1_2017) == *this->mar_1_2016 &&
            DateUtil::decrease_year(*this->mar_1_2018) == *this->mar_1_2017 &&
            DateUtil::decrease_year(*this->mar_1_2016) == *this->mar_1_2015)
}

void DateUtilTest::test_is_leap() {
    Test::print("test_is_leap ");
    ASSERT (DateUtil::is_leap(this->mar_1_2016->getYear()) &&
            DateUtil::is_leap(this->aug_20_2000->getYear()) &&
            !DateUtil::is_leap(this->jan_12_2015->getYear()))
}
