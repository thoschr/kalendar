#include "linuxnotifymanager.h"

LinuxNotifyManager::LinuxNotifyManager()
{

}

/* Send a notification with all the future events starting from the current date to the specified day */
bool LinuxNotifyManager::notifyEvents(int day) {
    DateUtil du;
    Date today = du.get_current_date();

}
