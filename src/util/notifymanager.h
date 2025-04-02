#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H


class NotifyManager
{
public:
    virtual ~NotifyManager() {}
    virtual bool notifyEvents(int day) = 0;
};

#endif // NOTIFYMANAGER_H
