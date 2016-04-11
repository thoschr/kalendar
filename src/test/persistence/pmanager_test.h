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
    Event *specialchars_event;
    Event *invalid_time_event;
    Event *event_with_invalid_category;
    Event *event_with_null_category;
    Category *valid_category;
    Category *valid_category_2;
    Category *valid_default_category;
    Category *noname_category;
    Category *specialchars_category;

public:
    PManagerTest();
    ~PManagerTest();
    void test_all();
    void test_add_event();
    void test_remove_event();
    void test_add_category();
    void test_get_events_of_month();
    void test_remove_category();
    void test_get_categories();
    void test_remove_all();
    void test_get_category();
    void test_edit_event();
    void test_get_all_events();
    void test_remove_past_events();
    void test_edit_category();
    void test_import_db();
    void test_export_db();
    void test_import_db_iCal_format();
    void test_export_db_iCal_format();
};

#endif // PMANAGERTEST_H
