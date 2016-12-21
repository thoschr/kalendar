#ifndef LINUXNOTIFYMANAGER_H
#define LINUXNOTIFYMANAGER_H

#include "notifymanager.h"
#include "dateutil.h"

class LinuxNotifyManager : public NotifyManager
{
public:
    LinuxNotifyManager();
    bool notifyEvents(int day);
};

#endif // LINUXNOTIFYMANAGER_H
