#ifndef PMANAGER_H
#define PMANAGER_H

#include <sqlite3.h> /* Version: 3.9.2 */
#include <stdio.h>
#include <QDir>
#include <QFile>
#include <list>
#include <ctime>
#include "../event.h"

using namespace std;

class PManager
{
public:
    PManager();
    ~PManager();
    bool add_event (Event *e);
    //bool remove_event(Event *e);
    list<Event *> get_events_of_month(int month, int year);
    bool add_category (Category *c);
    list<Category*> get_Categories();
private:
    sqlite3 *db;
};

#endif // PMANAGER_H
