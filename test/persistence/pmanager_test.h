#ifndef PMANAGERTEST_H
#define PMANAGERTEST_H

#include <stdlib.h>

#include "../test.h"
#include "../../persistence/pmanager.h"

class PManagerTest
{
private:
    Event *valid_event;
    Event *noname_event;
    Event *invalid_time_event;
public:
    PManagerTest();
    ~PManagerTest();
    void test_all();
    void test_pmanager_add_event();
    void test_pmanager_remove_event();
    void test_pmanager_add_category();
    void test_pmanager_get_events_of_month();
    void test_pmanager_remove_category();
    void test_pmanager_get_categories();
    void test_pmanager_remove_all();
};

#endif // PMANAGERTEST_H
