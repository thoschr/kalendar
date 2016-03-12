#include "pmanager_test.h"

PManagerTest::PManagerTest()
{
    unsigned long timestamp = (unsigned long) time(NULL);
    string test("test");
    this->valid_event = new Event(0, test, test, test, timestamp, timestamp + 100);
    /* Invalid Events */
    this->noname_event = new Event(1, string(""), test, test, timestamp, timestamp + 100);
    this->invalid_time_event = new Event(1, test, test, test, timestamp, timestamp - 100);

}

PManagerTest::~PManagerTest() {
    delete this->valid_event;
    delete this->noname_event;
    delete this->invalid_time_event;
}

void PManagerTest::test_all() {
    test_pmanager_remove_all();
    test_pmanager_add_event();
}

void PManagerTest::test_pmanager_remove_all() {
    Test::print("test_pmanager_remove_all ");
    PManager pm;
    pm.remove_all() ? Test::print_green("passed\n") : Test::print_red("failed\n");
}

void PManagerTest::test_pmanager_add_event() {
    Test::print("test_pmanager_add_event ");
    PManager pm;
    if ((!(pm.add_event(this->noname_event))) &&
       (!(pm.add_event(this->invalid_time_event))) &&
       (pm.add_event(this->valid_event)))
           Test::print_green("passed\n");
    else
           Test::print_red("failed\n");
    pm.remove_all();
}


