#include "pmanager_test.h"

PManagerTest::PManagerTest()
{
    unsigned long timestamp = (unsigned long) time(NULL);
    string test("test");
    this->event = new Event(0, test, test, test, timestamp, timestamp + 100);
}

PManagerTest::~PManagerTest() {
    delete this->event;
}

void PManagerTest::test_all() {
    test_pmanager_add_event();
}

void PManagerTest::test_pmanager_add_event() {
    Test::print("test_pmanager_add_event ");
    PManager pm;
    pm.add_event(this->event) ? Test::print_green("passed\n") : Test::print_red("failed\n");
}


