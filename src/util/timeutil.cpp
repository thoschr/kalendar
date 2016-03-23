#include "timeutil.h"

#include <QDebug>

static string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
static string week_days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

TimeUtil::TimeUtil()
{

}

Time TimeUtil::get_current_time() {
    time_t timestamp = time(NULL);
    struct tm *current_time = localtime(&timestamp);
    return Time(current_time->tm_mday, current_time->tm_wday ?: 7, current_time->tm_mon + 1, current_time->tm_year + 1900);
}

string TimeUtil::get_literal_month(int m) {
    if ((m < 1) || (m > 12))
        return string("");
    else
        return months[m-1];
}

string TimeUtil::numeric2literal_day_of_week(int d) {
    if ((d < 1) || (d > 7))
        return string("");
    else
        return week_days[d-1];
}

int TimeUtil::literal2numeric_day_of_week(string d) {
    int i = 0;
    for (i = 0; i < 7; i++) {
        if (d == week_days[i])
            return i+1;
    }
    return -1;
}

int TimeUtil::get_days_in_month(int month, int year) {
    int numberOfDays;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        numberOfDays = 30;
    else if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            numberOfDays = 29;
        else
            numberOfDays = 28;
    }
    else
      numberOfDays = 31;
    return numberOfDays;
}

Time TimeUtil::time_from_timestamp(unsigned long timestamp) {
    const time_t t = static_cast<const time_t> (timestamp);
    struct tm *tm = localtime(&t);
    Time time(tm->tm_mday, tm->tm_wday + 1, tm->tm_mon + 1, tm->tm_year + 1900);
    //free(tm); segfault?
    return time;
}

Time TimeUtil::get_first_day_of_month(Time &time) {
    int wday = time.getWeekDay() - (time.getMonthDay() % 7) + 1;
    //Normalization
    if (wday <= 0) wday += 7;
    else if (wday > 7) wday -= 7;
    return Time(1, wday, time.getMonth(), time.getYear());
}

Time TimeUtil::get_last_day_of_month(Time &time) {
    Time first_day = get_first_day_of_month(time);
    int tot_days = get_days_in_month(time.getMonth(), time.getYear());
    int wday = (tot_days % 7 ?: 7) + first_day.getWeekDay() - 1;
    return Time(tot_days, wday, time.getMonth(), time.getYear());
}

//TODO write tests
//TODO fix this functions, when you change month (and/or year) you need to change alse the weekday and put it to the right week day
//for example 16 of march has a differente week day from 16 of february
//Assume to get a valid time (i.e. no negative numbers, etc.)
Time TimeUtil::increase_month(Time time) {
    Time last_day_curr_month = get_last_day_of_month(time);
    Time first_day_next_month;
    if (time.getMonth() < 12) {
        first_day_next_month = Time(1, (last_day_curr_month.getWeekDay() % 7) + 1, time.getMonth() + 1, time.getYear());
    } else { //Go to next year
        first_day_next_month = Time(1, (last_day_curr_month.getWeekDay() % 7) + 1, 1, time.getYear() + 1);
    }
    return Time(time.getMonthDay(), (time.getMonthDay() % 7 ?: 7) + first_day_next_month.getWeekDay() - 1, first_day_next_month.getMonth(), first_day_next_month.getYear());
}

//Assume to get a valid time (i.e. no negative numbers, etc.)
Time TimeUtil::decrease_month(Time time) {
    Time first_day_curr_month = get_first_day_of_month(time);
    Time last_day_prev_month;
    if (time.getMonth() > 1) {
        last_day_prev_month = Time(get_days_in_month(time.getMonth() - 1, time.getYear()), first_day_curr_month.getWeekDay() - 1 ?: 7, time.getMonth() - 1, time.getYear());
    } else { //Go to previous year
        last_day_prev_month = Time(get_days_in_month(12, time.getYear() - 1), first_day_curr_month.getWeekDay() - 1 ?: 7, 12, time.getYear() - 1);
    }
    Time first_day_prev_month = get_first_day_of_month(last_day_prev_month);
    return Time(time.getMonthDay(), (time.getMonthDay() % 7 ?: 7) + first_day_prev_month.getWeekDay() - 1, last_day_prev_month.getMonth(), last_day_prev_month.getYear());
}
