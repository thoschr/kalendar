#include "timeutil.h"

TimeUtil::TimeUtil()
{

}

Time TimeUtil::get_current_time() {
    time_t timestamp = time(NULL);
    struct tm *current_time = localtime(&timestamp);
    return Time(current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900);
}
