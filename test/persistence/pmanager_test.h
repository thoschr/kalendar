#ifndef PMANAGERTEST_H
#define PMANAGERTEST_H

#include <stdlib.h>

#include "../test.h"
#include "../../persistence/pmanager.h"

class PManagerTest
{
private:
    Event *valid_event;
    Event *valid_event_2;
    Event *noname_event;
    Event *invalid_time_event;
    Category *valid_category;
    Category *valid_category_2;
    Category *valid_default_category;
    Category *noname_category;
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
