#ifndef LINUXNOTIFYMANAGER_H
#define LINUXNOTIFYMANAGER_H

#include <QProcess>
#include "../persistence/pmanager.h"
#include "notifymanager.h"
#include "dateutil.h"

#define ICON_NAME "appointment-new" /* TODO: choose a better icon */

class LinuxNotifyManager : public NotifyManager
{
public:
    LinuxNotifyManager();
    bool notifyEvents(int dayoffset);
private:
    bool notifyEvent(Event *e);
};

#endif // LINUXNOTIFYMANAGER_H
