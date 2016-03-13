#include "timeutil.h"

static string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
static string week_days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

TimeUtil::TimeUtil()
{

}

Time TimeUtil::get_current_time() {
    time_t timestamp = time(NULL);
    struct tm *current_time = localtime(&timestamp);
    return Time(current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900);
}

string TimeUtil::get_literal_month(int m) {
    if ((m < 1) || (m > 12))
        return NULL;
    else
        return months[m-1];
}

string TimeUtil::get_literal_day_of_week(int d) {
    if ((d < 1) || (d > 7))
        return NULL;
    else
        return week_days[d-1];
}

int TimeUtil::get_numeric_day_of_week(string d) {
    int i = 0;
    for (i = 0; i < 7; i++) {
        if (d == week_days[i])
            return i+1;
    }
    return -1;
}
