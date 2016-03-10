#ifndef PMANAGERTEST_H
#define PMANAGERTEST_H

#include "../test.h"
#include "../../persistence/pmanager.h"

class PManagerTest
{
public:
    PManagerTest();
    void test_all();
    void test_pmanager_initialization_with_db();
    void test_pmanager_initialization_without_db();
    void test_pmanager_add_event();
    void test_pmanager_remove_event();
    void test_pmanager_add_category();
    void test_pmanager_get_events_of_month();
    void test_pmanager_remove_category();
    void test_pmanager_get_categories();
};

#endif // PMANAGERTEST_H
