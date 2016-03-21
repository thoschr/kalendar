#include "timeutil.h"

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
    int week_day = time.getWeekDay() - (time.getMonthDay() % 7) + 1;
    return Time(1, week_day, time.getMonth(), time.getYear());
}

//TODO write tests
//TODO fix this functions, when you change month (and/or year) you need to change alse the weekday and put it to the right week day
//for example 16 of march has a differente week day from 16 of february
//Assume to get a valid time (i.e. no negative numbers, etc.)
Time TimeUtil::increase_month(Time time) {
    //The months haven't the same number of days, so I need to use a day that have all months, like the first day.
    time = get_first_day_of_month(time);
    if (time.getMonth() < 12) {
        time.setMonth(time.getMonth() + 1);
        return time;
    } else { //Go to next year
        time.setMonth(1);
        time.setYear(time.getYear() + 1);
        return time;
    }
}

//Assume to get a valid time (i.e. no negative numbers, etc.)
Time TimeUtil::decrease_month(Time time) {
    if (time.getMonth() > 1) {
        time.setMonth(time.getMonth() - 1);
        return time;
    } else { //Go to previous year
        time.setMonth(12);
        time.setYear(time.getYear() - 1);
        return time;
    }
}
