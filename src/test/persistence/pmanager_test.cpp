#include "pmanager_test.h"

PManagerTest::PManagerTest()
{
    unsigned long timestamp = (unsigned long) time(NULL);
    string test("test_string");
    this->valid_event = new Event(0, test, test, test, timestamp, timestamp + 100);
    this->valid_event_2 = new Event(100, test, test, test, timestamp, timestamp + 100000);
    /* Invalid Events */
    this->noname_event = new Event(1, string(""), test, test, timestamp, timestamp + 100);
    this->invalid_time_event = new Event(1, test, test, test, timestamp, timestamp - 100);
    /* Categories */
    this->valid_default_category = new Category(1, test, test);
    this->valid_category = new Category(0, test, test);
    this->valid_category_2 = new Category(100, test, test);
    this->noname_category = new Category(0, string(""), test);
}

PManagerTest::~PManagerTest() {
    delete this->valid_event;
    delete this->valid_event_2;
    delete this->noname_event;
    delete this->invalid_time_event;
    delete this->noname_category;
    delete this->valid_category;
    delete this->valid_category_2;
    delete this->valid_default_category;
}

void PManagerTest::test_all() {
    test_remove_all();
    test_add_event();
    test_get_events_of_month();
    test_remove_event();
    test_add_category();
    test_get_categories();
    test_remove_category();
}

void PManagerTest::test_remove_all() {
    Test::print("test_remove_all ");
    PManager pm;
    ASSERT (pm.remove_all())
}

void PManagerTest::test_add_event() {
    Test::print("test_add_event ");
    PManager pm;
    ASSERT ((!(pm.add_event(this->noname_event))) &&
           (!(pm.add_event(this->invalid_time_event))) &&
           (pm.add_event(this->valid_event)))
    pm.remove_all();
}

void PManagerTest::test_get_events_of_month() {
    Test::print("test_get_events_of_month ");
    bool ret = false;
    PManager pm;
    pm.add_event(this->valid_event);
    time_t timestamp = time(NULL);
    struct tm *current_time = localtime(&timestamp);
    list<Event*> events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900); // tm_mon is from 0 to 11, we need to have 1 - 12
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret = this->valid_event->equals(**it); // *it has type Event*
        delete *it;
    }
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_remove_event() {
    Test::print("test_remove_event ");
    bool ret = false;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    pm.remove_event(this->valid_event);
    time_t timestamp = time(NULL);
    struct tm *current_time = localtime(&timestamp);
    list<Event*> events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900);
    if (events.size() == 1) {
        list<Event*>::iterator it = events.begin();
        ret = this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    }
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_add_category() {
    Test::print("test_add_category ");
    PManager pm;
    ASSERT ((!(pm.add_category(this->noname_category))) && (pm.add_category(this->valid_category)))
    pm.remove_all();
}

void PManagerTest::test_get_categories() {
    Test::print("test_get_categories ");
    bool ret = false;
    PManager pm;
    pm.add_category(this->valid_category);
    list<Category*> categories = pm.get_categories();
    if (!(categories.empty())) {
        list<Category*>::iterator it = categories.begin();
        ret = this->valid_category->equals(**it); // *it has type Category*
        delete *it;
    }
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_remove_category() {
    Test::print("test_remove_category ");
    bool ret = false;
    PManager pm;
    pm.add_category(this->valid_category);
    pm.add_category(this->valid_category_2);
    pm.remove_category(this->valid_category);
    pm.remove_category(this->valid_default_category); //Delete the default category
    list<Category*> categories = pm.get_categories();
    if (categories.size() == 1) {
        list<Category*>::iterator it = categories.begin();
        ret = this->valid_category_2->equals(**it); // *it has type Category*
        delete *it;
    }
    ASSERT (ret)
    pm.remove_all();
}
